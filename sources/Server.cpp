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

Server::Server(const std::vector<Config>& vec) : _serverBlocks(vec), _addrInfo{0}
{	
	try
	{
		for (size_t i = 0; i < _serverBlocks.size(); ++i)
 		{
			setupAddrInfo(&_serverBlocks[i]);
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

	status = getaddrinfo(config->_serverName.c_str(), config->_serverPort.c_str(), &hints, &_addrInfo); //?
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
	close(_Connections[i]._clientFD);
	_Connections.erase(_Connections.begin() + i);
	_pollFDs.erase(_pollFDs.begin() + i);
}


#define TMP_POLL_TIME 5000

void	Server::main_server_loop()
{
	int	size = _pollFDs.size();
	
	poll(_pollFDs.data(), size, TMP_POLL_TIME);
	for (size_t i = 0; i < size; ++i)
	{		
		if (_pollFDs[i].revents & POLLIN)
		{
			if (_Connections[i]._isServerSocket) 
				acceptNewConnects(i);
			else
				_Connections[i]._request.readRequest();
		}
		else if ((_pollFDs[i].revents & POLLOUT) && _Connections[i]._request._doneReading)
		{
			responseHandler(&_Connections[i]._request);
			if (_Connections[i]._keepOpen) 
				_Connections[i]._request = Request(_Connections[i]._clientFD); 
			else
				close_connect(i);
		}
	}
}


void Server::acceptNewConnects(int i)
{
	int clientFD = accept(_pollFDs[i].fd, nullptr, nullptr);
	if (clientFD > 0)
	{
		std::cout << "new connection! :)" << std::endl;
		_pollFDs.emplace_back(pollfd{clientFD, POLLIN | POLLOUT | POLLERR | POLLHUP, 0});
		_Connections.emplace_back(_Connections[i]._config, clientFD, false);
	}
	else
		std::cout << "NOT ACCEPTED" << std::endl;
}