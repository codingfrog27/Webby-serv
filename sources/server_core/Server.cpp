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
void writeClientFD(int clientFD, int i);

void writeClientFD(int clientFD, int i)
{
	std::ofstream outFile("clientFD_log.txt", std::ios::app);
		outFile << "Accepted new connection with clientFD: " << clientFD <<\
		" on index" << i << std::endl;
		// Example of using the address of clientFD for logging or other purposes
		// outFile << "Address of clientFD: " << &clientFD << std::endl;
		outFile.close();
}


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Server::Server(std::vector<Config>& vec) : _serverBlocks(vec), _addrInfo{0}
{
	try
	{
		std::cout << "hoi" << std::endl;
		_Connections.reserve(100);
		int	FD;
		_serverSockets.reserve(_serverBlocks.size());
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
		std::cout << e.what() << "\nserver setup failed, shutting down" << std::endl;
		exit(1);
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

	// std::cout << "our addr info args ARE\n\nname = " << config->_serverName << \
	// " listen = " << config->_listen.c_str() << "\n\n" << std::endl;

	status = getaddrinfo(config->_serverName.c_str(), config->_listen.c_str(), &hints, &_addrInfo); //can throw??
	if (status != 0) {
		std::cout << RED "cant connect " <<  config->_serverName <<  gai_strerror(status) << RESET << std::endl;
		std::cout << config->_serverName.c_str() << config->_serverPort.c_str() << std::endl;
		throw std::runtime_error(std::string("getaddrinfo error: ") + gai_strerror(status));
	}
	// else
	// 	std::cout << GREEN "Server block " << config->_serverName << "opened!" RESET << std::endl;

}


Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

	// signal(SIGPIPE, SIG_IGN);


void	Server::main_server_loop()
{
	size_t	size;
	while (1)
	{
		size = _pollFDs.size();
		if (poll(_pollFDs.data(), size, 0) == 0)
			continue; //throw server close error on >0
		for (size_t i = 0; i < size; i++)
		{
			Connection &current = _Connections.at(_pollFDs[i].fd);
			//should be find to check for nonexisting connects (though also shouldnt happen)
			if (current._isClientSocket)
				current.connAction(_pollFDs[i], *this);	
			else if (_pollFDs[i].revents & POLLIN)
					current._wantsNewConnect = true;
		}
		acceptNewConnects(size); //could mb be part of main loop after all?
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

	}
}


// if (_pollFDs[i].fd != _Connections[i]._clientFD)
// {
// 	std::cout << "FD MISMATACH OH NO" << std::endl;
// 	NicePrint::promptEnter();
// }
// close(_Connections[i]._clientFD);
// _Connections.erase(_Connections.begin() + i);
// _pollFDs.erase(_pollFDs.begin() + i);
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
				std::cout << "WHAT THE HELLL" << std::endl;
			else {
				_Connections.erase(fd);
				std::cout << "CLOSED CONNECT with FD: " \
				<< fd << " With index: " << std::endl;
				return;
			}
		}
		it++;
		// itc++;
	}
	std::cout << "bro \nI can't close connect" << fd << std::endl;
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
				std::cerr << RED "Couldn't accept connection on socket FD " << _pollFDs[i].fd \
				<< " error == " << strerror(errno) << std::endl;
				break;
			}
			else {
				std::cout << GREEN "new connection! FD == " RESET << clientFD <<  std::endl;
				current._wantsNewConnect = false; //move
				_pollFDs.emplace_back(\
						pollfd{clientFD, POLLIN | POLLOUT | POLLERR | POLLHUP, 0});
				_Connections.emplace(clientFD, \
					Connection{current._config, clientFD, true});
				writeClientFD(clientFD, i);
			}
		}
	}
}
