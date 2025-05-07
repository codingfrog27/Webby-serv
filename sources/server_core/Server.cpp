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
#include <signal.h>

// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Server::Server(std::vector<Config>& vec) : _serverBlocks(vec), _addrInfo{0}
{
	try
	{
		signal(SIGPIPE, SIG_IGN);
		int	FD = -1;
		_Connections.reserve(100);
		_serverSockets.reserve(_serverBlocks.size());
		for (size_t i = 0; i < _serverBlocks.size(); ++i)
 		{
			setupAddrInfo(&_serverBlocks[i]);
			_serverSockets.emplace_back(&_serverBlocks[i], _addrInfo);
			FD = _serverSockets[i]._socketFd;
			_pollFDs.emplace_back(pollfd{FD, POLLIN | POLLERR,  0});
			_Connections.emplace(FD, Connection{&_serverBlocks[i], *this, FD, false});
		}
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	std::cout << GREEN << "Server is running :)" << RESET << std::endl;
}


// AF_UNSPEC == can be either IPv4 or IPv6
// SOCK_STREAM == Specifies we want a TCP socket type
// AI_PASSIVE == Provides additional options (AI_PASSIVE for binding to all network interfaces)
// IPPROTO_TCP == Specifies the protocol
void	Server::setupAddrInfo(Config *config)
{
	addrinfo hints = {};
	int status;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	if (_addrInfo)
	{
		freeaddrinfo(_addrInfo);
		_addrInfo = nullptr;
	}

	status = getaddrinfo(config->_host.c_str(), config->_listen.c_str(), &hints, &_addrInfo);
	if (status != 0){
		std::cerr << RED "getaddrinfo error: " << gai_strerror(status) << RESET "\n\nClosing server" << std::endl;
		exit(1);
	}

}

Server::~Server(void)
{
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
	killAllCGIProcesses();
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

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
			auto it = _Connections.find(_pollFDs[i].fd);
			if (it != _Connections.end()) 
			{
			    Connection &current = it->second;
				if (current._isClientSocket)
					current.connectionAction(_pollFDs[i], *this);
				else if (_pollFDs[i].revents & POLLIN)
						current._wantsNewConnect = true;
			} 
			else 
			    continue;
		}
		acceptNewConnects(size);
		for (size_t i = 0; i < size;)
		{
			auto it = _Connections.find(_pollFDs[i].fd);
			if (it != _Connections.end()) 
			{
			    Connection &current = it->second;
				if (current._CStatus == connectStatus::CONNECT_CLOSED || \
					current._CStatus == connectStatus::FINISHED)
					{
						close_connect(current._clientFD);
						size--;
					}
				else
					i++;
			}
			else 
			{
				_pollFDs.erase(_pollFDs.begin() + i);
				size--;
			    continue;
			}
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
			if (_Connections.at(fd)._clientFD != fd)
				std::cerr << RED "connection and pollfd misallignment" RESET << std::endl;
			else 
			{
				_Connections.erase(fd);
				std::cout << "CLOSED CONNECT with FD: " \
				<< fd << std::endl;
				return;
			}
		}
		it++;
	}
	std::cerr << RED "couldn't find connection to close" RESET << std::endl;
}


void Server::handleCGIPollEvents() {
	size_t	size = _CGIPollFDs.size();
	if (poll(_CGIPollFDs.data(), size, 0) == 0)
		return ;
	for (size_t i = 0; i < size; i++){
		CGI *cgi = _CGIMap[_CGIPollFDs[i].fd].get();
		if (cgi == nullptr)
			continue;

		auto it = _Connections.find(cgi->getClientFD());
		if (it == _Connections.end())
			continue;
		Connection &connection = it->second;
		if (_CGIPollFDs[i].fd == cgi->getFdIn() && _CGIPollFDs[i].revents & POLLOUT){
			cgi->writeToCGI(&connection._request, &connection._response);
		}
		else if (cgi->getChildIsRunningStatus() == false || !cgi->childIsRunning(&connection._response))
		{
			if (_CGIPollFDs[i].fd == cgi->getFdOut() && _CGIPollFDs[i].revents & POLLIN){
				cgi->readFromCGI(&connection._response);
			}
			else if (_CGIPollFDs[i].fd == cgi->getFdError() && _CGIPollFDs[i].revents & POLLIN){
				cgi->readErrorFromCGI(&connection._response);
			}
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
					connection.removeCGIFromEverywhere();
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
		auto it = _Connections.find(_pollFDs[i].fd);
		if (it != _Connections.end()) 
		{
		    Connection &current = it->second;
			if (current._wantsNewConnect == true)
			{
				clientFD = accept(_pollFDs[i].fd, nullptr, nullptr);
				if (clientFD <= 0)
				{
					std::cout << "NOT ACCEPTED" << clientFD << std::endl;
					NicePrint::promptEnter();
					break;
				}
				else {
					std::cout << GREEN "new connection " RESET << "FD == " << clientFD <<  std::endl;
					current._wantsNewConnect = false;
					_pollFDs.emplace_back(\
							pollfd{clientFD, POLLIN | POLLOUT | POLLERR | POLLHUP, 0});
					_Connections.emplace(clientFD, \
						Connection{current._config, *this, clientFD, true});
				}
			}
		} 
		else 
		    continue;
	}
}

void Server::killAllCGIProcesses()
{
	for (auto &cgi : _CGIMap)
	{
		cgi.second->killChild();
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
