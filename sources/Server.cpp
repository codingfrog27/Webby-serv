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



	// _Connections.shrink_to_fit();
	// shutdown(_Connections[i]._clientFD, SHUT_RDWR); //!!!
void	Server::close_connect(int fd)
{
	//  std::ofstream outFile("clientFD_log.txt", std::ios::app);
	// 	outFile << "CLOSED CONNECT with FD: " \
	// 	<< _Connections[i]._clientFD << " With index: " << i << std::endl;
	// if (_pollFDs[i].fd != _Connections[i]._clientFD)
	// {
	// 	std::cout << "FD MISMATACH OH NO" << std::endl;
	// 	NicePrint::promptEnter();
	// }
	// close(_Connections[i]._clientFD);
	// _Connections.erase(_Connections.begin() + i);
	// _pollFDs.erase(_pollFDs.begin() + i);

	std::vector<pollfd>::iterator it = _pollFDs.begin();
	std::vector<Connection>::iterator itc = _Connections.begin();
	while ( it != _pollFDs.end())
	{
		if (it->fd == fd)
		{
			close(fd);
			_pollFDs.erase(it);
			_Connections.erase(itc);
			break ;
		}
		it++;
		itc++;
	}
	if (it == _pollFDs.end())
	{
		std::cout << "FUUUUUUUUCK" << std::endl;
	}

}


#define TMP_POLL_TIME 500000
// #include <signal.h>

void	Server::main_server_loop()
{
	size_t	size;
	size_t	i;
	size_t	eventsAmount;
	// signal(SIGPIPE, SIG_IGN);
	while (1)
	{
		//if poll timeout-> get current time
		size = _pollFDs.size();
		// std::cout << "This is start size: "<< size << std::endl;
		eventsAmount = poll(_pollFDs.data(), size, 0);
		if (eventsAmount == 0)
			continue;
		i = 0;
		while (i < size)
		{
			_Connections[i].connectionAction(_pollFDs[i]);
			i++;
		}
		i = 0;
		while (i < size)
		{
			if (_Connections[i]._wantsNewConnect == true) {
				acceptNewConnects(i);
				_Connections[i]._wantsNewConnect = false;
			}
			i++;
		}
		i = 0;
		while (i < size)
		{
			// if (_Connections[i]._CStatus == connectStatus::SERV_SOCKET) 
			// {    
			//     std::cout << "This is 1" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::IDLE){
			//     std::cout << "This is 2" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::READING) {
			//     std::cout << "This is 3" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::REQ_ERR) {
			//     std::cout << "This is 4" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::CONNECT_CLOSED) {
			//     std::cout << "This is 5" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::DONE_READING) {
			//     std::cout << "This is 6" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::DONE_READING_CGI) {
			//     std::cout << "This is 7" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::RESPONDING) {
			//     std::cout << "This is 8" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::SERVER_ERR) {
			//     std::cout << "This is 9" << std::endl;
			// } else if (_Connections[i]._CStatus == connectStatus::FINISHED) {
			//     std::cout << "This is 10" << std::endl;
			// }
				if (_Connections[i]._CStatus == connectStatus::CONNECT_CLOSED || \
					_Connections[i]._CStatus == connectStatus::FINISHED)
					{
						close_connect(_Connections[i]._clientFD);
						size--;
					}
				else
					i++;
			// }
		}
	}
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