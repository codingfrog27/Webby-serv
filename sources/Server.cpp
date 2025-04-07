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

// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Server::Server(std::vector<Config>& vec) : _serverBlocks(vec), _addrInfo{0}
{
	try
	{
		_Connections.reserve(100);
		int	FD;
		for (size_t i = 0; i < _serverBlocks.size(); ++i)
 		{
			setupAddrInfo(&_serverBlocks[i]);
			_serverSockets.emplace_back(&_serverBlocks[i], _addrInfo);
			FD = _serverSockets[i]._socketFd;
			_pollFDs.emplace_back(pollfd{FD, POLLIN | POLLERR,  0});
			_Connections.emplace(FD, Connection{&_serverBlocks[i], FD, false});
		}
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
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
	if (status != 0)
		throw std::runtime_error(std::string("getaddrinfo error: ") + gai_strerror(status));

}


Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}


void	Server::main_server_loop()
{
	size_t	size;
	while (1)
	{
		size = _pollFDs.size();
		if (poll(_pollFDs.data(), size, 0) == 0)
			continue;
		for (size_t i = 0; i < size; i++)
		{
			Connection &current = _Connections.at(_pollFDs[i].fd);
			if (current._isClientSocket)
				current.connectionAction(_pollFDs[i], *this);
			else if (_pollFDs[i].revents & POLLIN)
					current._wantsNewConnect = true;
			
		}
		acceptNewConnects(size); 
		for (size_t i = 0; i < size;)
		{
			Connection &current = _Connections.at(_pollFDs[i].fd);
			if (current._CStatus == connectStatus::CONNECT_CLOSED || \
				current._CStatus == connectStatus::FINISHED)
				{
					close_connect(current._clientFD);
					size--;
				}
			else
				i++;
		}
		handleCGIPollEvents();
	}
}

void	Server::close_connect(int fd)
{
	std::vector<pollfd>::iterator it = _pollFDs.begin();
	while (it != _pollFDs.end())
	{
		if (it->fd == fd)
		{
			close(fd);
			_pollFDs.erase(it);
			if (_Connections.at(fd)._clientFD == fd)
			{
				_Connections.erase(fd);
				return;
			}
		}
		it++;
	}
}



void Server::handleCGIPollEvents() {
	size_t	size;
	size = _CGIPollFDs.size();
	if (poll(_CGIPollFDs.data(), size, 0) == 0)
		return ;
	for (size_t i = 0; i < size; i++)
	{
		CGI *cgi = _CGIMap[_CGIPollFDs[i].fd].get();
		if (cgi == nullptr)
			continue;
		Connection &connection = _Connections.at(cgi->getClientFD());
		if (_CGIPollFDs[i].fd == cgi->getFdIn() && _CGIPollFDs[i].revents & POLLOUT)
			cgi->writeToCGI(&connection._request, &connection._response);
		else if (cgi->getChildIsRunningStatus() == false || !cgi->childIsRunning(&connection._response))
		{
			if (_CGIPollFDs[i].fd == cgi->getFdOut() && _CGIPollFDs[i].revents & POLLIN)
				cgi->readFromCGI(&connection._response);
			else if (_CGIPollFDs[i].fd == cgi->getFdError() && _CGIPollFDs[i].revents & POLLIN)
				cgi->readErrorFromCGI(&connection._response);
			if (_CGIPollFDs[i].revents & POLLHUP || (cgi->getCGIHandlerStatus() == CGIHandlerStatus::FINISHED && !(_CGIPollFDs[i].revents & POLLIN)))
			{
				if (_CGIPollFDs[i].revents & POLLHUP)
					close(_CGIPollFDs[i].fd );
				_CGIMap.erase(_CGIPollFDs[i].fd);
				_CGIPollFDs.erase(_CGIPollFDs.begin() + i);
				size--;
				i--;
				if (cgi->getCGIHandlerStatus() == CGIHandlerStatus::FINISHED){
					connection._CStatus = connectStatus::RESPONDING;
					connection._response.setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
					connection.removeCGIFromEverywhere(*this);
				}
				continue;
			}
		}
	}
}

void Server::acceptNewConnects(size_t size)
{
	int clientFD = 0;
	for (size_t i = 0; i < size; i++)
	{
		Connection &current = _Connections.at(_pollFDs[i].fd);
		if (current._wantsNewConnect == true)
		{
			clientFD = accept(_pollFDs[i].fd, nullptr, nullptr);
			if (clientFD <= 0)
			{
				std::cout << "NOT ACCEPTED" << clientFD << std::endl;
				break;
			}
			else {
				// std::cout << GREEN "new connection!" RESET << clientFD <<  std::endl;
				current._wantsNewConnect = false;
				_pollFDs.emplace_back(\
						pollfd{clientFD, POLLIN | POLLOUT | POLLERR | POLLHUP, 0});
				_Connections.emplace(clientFD, \
					Connection{current._config, clientFD, true});
			}
		}
	}
}

std::vector<pollfd>		&Server::getCGIPollFDs(void)
{
	return (_CGIPollFDs);
}

std::unordered_map<int, std::shared_ptr<CGI>>	&Server::getCGIMap(void)
{
	return (_CGIMap);
}