/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/03 14:32:11 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/07 18:32:31 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "everything.hpp"

#include <iostream>

#define MAX_CLIENT 300


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Server::Server(std::vector<Config>& vec) : _serverBlocks(vec), _addrInfo{0}
{
	try
	{
		_Connections.reserve(100);
		for (size_t i = 0; i < _serverBlocks.size(); ++i)
 		{
			setupAddrInfo(&_serverBlocks[i]);
			std::cout << "serblock" << i << "made" << std::endl;
			_serverSockets.emplace_back(&_serverBlocks[i], _addrInfo);
			_pollFDs.emplace_back(pollfd{_serverSockets[i]._socketFd, POLLIN | POLLERR,  0});
			_Connections.emplace_back(&_serverBlocks[i], _serverSockets[i]._socketFd, true);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << GREEN << "Server is running :)" << RESET << std::endl;
}


// AF_UNSPEC = don't care IPv4 or IPv6
// SOCK_STREAM; //Specifies the socket type (SOCK_STREAM for TCP)
// AI_PASSIVE; //Provides additional options (AI_PASSIVE for binding to all network interfaces)
// IPPROTO_TCP; //Specifies the protocol
void	Server::setupAddrInfo(Config *config)
{
	addrinfo hints;
	int status;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo(config->_serverName.c_str(), config->_listen.c_str(), &hints, &_addrInfo);
	// std::cout << config->_serverName.c_str() << config->_serverPort.c_str() << std::endl;
	if (status != 0)
		throw std::runtime_error(std::string("getaddrinfo error: ") + gai_strerror(status));

}


Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //



void	Server::close_connect(int i)
{
	 std::ofstream outFile("clientFD_log.txt", std::ios::app);
		outFile << "CLOSED CONNECT with FD: " \
		<< _Connections[i]._clientFD << " With index: " << i << std::endl;
	// NicePrint::promptEnter();
	close(_Connections[i]._clientFD);
	_Connections.erase(_Connections.begin() + i);
	_pollFDs.erase(_pollFDs.begin() + i);
}


#define TMP_POLL_TIME 500000

void	Server::main_server_loop()
{
	while (1)
	{
		//if poll timeout-> get current time
		size_t	size = _pollFDs.size();
		poll(_pollFDs.data(), size, -1);
		for (size_t i = 0; i < size; ++i)
		{
			connectionAction(_Connections[i], _pollFDs[i]);
		}
		for (size_t i = 0; i < size; i++)
		{
			if (_Connections[i]._wantsNewConnect == true) {
				acceptNewConnects(i);
				_Connections[i]._wantsNewConnect = false;
			}
		}
		for (size_t i = 0; i < size; i++)
		{
			if (_Connections[i]._CStatus == connectStatus::CONNECT_CLOSED || \
				_Connections[i]._CStatus == connectStatus::FINISHED)
				close_connect(i);
		}
	}
}

void	Server::connectionAction(Connection &connect, pollfd &poll)
{
	if (poll.revents & POLLIN && !connect._request._doneReading)
	{
		if (connect._isServerSocket) {
			connect._wantsNewConnect = true;
			return ;
		}
		else
			connect._CStatus = connect._request.readRequest();
	}
	if (connect._CStatus == connectStatus::CONNECT_CLOSED)
	{
		return;
		// std::cout << "bruh??" << std::endl;
		// close_connect(i);
	}
	if (connect._CStatus == connectStatus::DONE_READING) {
		connect._CStatus = connectStatus::RESPONDING;
		if (connect._request.getHeaderValue("Connection") == "keep-alive")
			connect._keepOpen = true;	
	}
	if ((poll.revents & POLLOUT) && connect._CStatus == connectStatus::RESPONDING) //crashes??
		connect._CStatus = responseHandler(&connect._request, &connect._response);
	if (connect._CStatus == connectStatus::FINISHED) //responseHandlerStatus::WRITING
	{
		if (connect._keepOpen){
			std::cout << "KEEPOPEN == ON" << std::endl;
			connect.resetRequest(connect._config, connect._clientFD);
			connect.resetResponse();
			connect._CStatus = connectStatus::IDLE;
		}
		else
			connect._CStatus = connectStatus::FINISHED;
	}
	// else if (isTimedOut(connect._startTime, connect._TimeoutTime))
	// 	close_connect(i); // has issues??
}

void writeClientFD(int clientFD, int i)
{
    std::ofstream outFile("clientFD_log.txt", std::ios::app);
		outFile << "Accepted new connection with clientFD: " << clientFD <<\
		" on index" << i << std::endl;
        // Example of using the address of clientFD for logging or other purposes
        // outFile << "Address of clientFD: " << &clientFD << std::endl;
        outFile.close();
}


void Server::acceptNewConnects(int i)
{
	int clientFD = accept(_pollFDs[i].fd, nullptr, nullptr); //timeout check??
	if (clientFD > 0)
	{
		// NicePrint::promptEnter(); //and donewriting
		_pollFDs.emplace_back(pollfd{clientFD, POLLIN | POLLOUT | POLLERR | POLLHUP, 0}); //frees???
		_Connections.emplace_back(_Connections[i]._config, clientFD, false);
		writeClientFD(clientFD, _Connections.size() - 1);
	}
	else
		std::cout << "NOT ACCEPTED" << std::endl;
}

//exits webserve after 1 response