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

	// signal(SIGPIPE, SIG_IGN);

	//refactor -> close connect loop 1 function, and for loops
		//if poll timeout-> get current time
		// std::cout << "This is start size: "<< size << std::endl;
		// PrintConnectionStatusses(size);
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
				current.connectionAction(_pollFDs[i], *this);
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
		handleCGIPollEvents();
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
	if (_pollFDs.size() != _Connections.size())
		std::cout << RED "VECTOR SIZE MISMAtCH BRO" RESET << std::endl;
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


//remove _CGIPollFDs[i] when finished in read or write function
void Server::handleCGIPollEvents() {
	size_t	size;
	size = _CGIPollFDs.size();
	if (poll(_CGIPollFDs.data(), size, 0) == 0)
		return ;
	for (size_t i = 0; i < size; i++){
		// std::cout << MAGENTA "CGI PollFD vector size in handleCGIPollEvents: " << _CGIPollFDs.size() << RESET << std::endl;
		// std::cout << "This is the fd left: " << _CGIPollFDs[i].fd << std::endl;
		CGI *cgi = _CGIMap[_CGIPollFDs[i].fd].get();
		// std::cout << "stdin: " << cgi->getFdIn() << std::endl;
		// std::cout << "stdout: " << cgi->getFdOut() << std::endl;
		// std::cout << "stderr: " << cgi->getFdError() << std::endl;
		if (cgi == nullptr){
			std::cout << "CGI is nullptr" << std::endl;
			continue;
		}
		Connection &connection = _Connections.at(cgi->getClientFD());
		if (_CGIPollFDs[i].fd == cgi->getFdIn() && _CGIPollFDs[i].revents & POLLOUT){
			std::cout << "fdin is pollout" << std::endl;
			cgi->writeToCGI(&connection._request, &connection._response);
		}
		else if (cgi->getChildIsRunningStatus() == false || !cgi->childIsRunning(&connection._response)){ //DOESNT GET HERE
			if (_CGIPollFDs[i].fd == cgi->getFdOut() && _CGIPollFDs[i].revents & POLLIN){
				std::cout << "fdout is pollin" << std::endl;
				cgi->readFromCGI(&connection._response);
			}
			else if (_CGIPollFDs[i].fd == cgi->getFdError() && _CGIPollFDs[i].revents & POLLIN){
				std::cout << "error fd is pollin" << std::endl;
				cgi->readErrorFromCGI(&connection._response);
			}
			if (_CGIPollFDs[i].revents & POLLHUP || (cgi->getCGIHandlerStatus() == CGIHandlerStatus::FINISHED && !(_CGIPollFDs[i].revents & POLLIN))){
				if (_CGIPollFDs[i].revents & POLLHUP)
					close(_CGIPollFDs[i].fd );
				_CGIMap.erase(_CGIPollFDs[i].fd);
				_CGIPollFDs.erase(_CGIPollFDs.begin() + i);
				size--;
				i--;
				if (cgi->getCGIHandlerStatus() == CGIHandlerStatus::FINISHED){
					connection._CStatus = connectStatus::RESPONDING;
					connection._response.setResponseHandlerStatus(responseHandlerStatus::READY_TO_WRITE);
					std::cout << MAGENTA "Response buffer: " << connection._response.getResponseBuffer() << RESET << std::endl;
				}
				continue;
			}
		}
	}
}

// void Server::closeCGIConnects(){
// 	size_t	size;
// 	size = _CGIPollFDs.size();
// 	for (size_t i = 0; i < size; i++){
// 		CGI *cgi = _CGIMap[_CGIPollFDs[i].fd].get();
// 		if (cgi == nullptr){
// 			std::cout << "CGI is nullptr" << std::endl;
// 			continue;
// 		}
// 		Connection &connection = _Connections.at(cgi->getClientFD());
// 		if (_CGIPollFDs[i].revents & POLLHUP || (cgi->getCGIHandlerStatus() == CGIHandlerStatus::CHILD_IS_FINISHED && !_CGIPollFDs[i].revents & POLLIN)){
// 			if (_CGIPollFDs[i].revents & POLLHUP)
// 				close(_CGIPollFDs[i].fd );
// 			_CGIMap.erase(_CGIPollFDs[i].fd);
// 			_CGIPollFDs.erase(_CGIPollFDs.begin() + i);
// 			size--;
// 			i--;
// 			continue;
// 		}
// 	}
// }

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
				NicePrint::promptEnter();
				break;
			}
			else {
				std::cout << GREEN "new connection!" RESET << clientFD <<  std::endl;
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

std::vector<pollfd>		&Server::getCGIPollFDs(void)
{
	return (_CGIPollFDs);
}

std::unordered_map<int, std::shared_ptr<CGI>>	&Server::getCGIMap(void)
{
	return (_CGIMap);
}



// void Server::PrintConnectionStatusses(size_t size)
// {
// 	for (size_t i = 0; i < size; i++)
// 	{
// 		// std::cout << "This is i: " << i << std::endl;
// 		// std::cout << "This is after close size: "<< size << std::endl;
// 		// Checking each possible enum value
// 		if (_Connections[i]._CStatus == connectStatus::SERV_SOCKET) {
// 			// Handle SERV_SOCKET status
// 			std::cout << "This is Server socket" << std::endl;
// 			// Code for handling SERV_SOCKET
// 		} else if (_Connections[i]._CStatus == connectStatus::IDLE) {
// 			// Handle IDLE status
// 			std::cout << "This is IDLE" << std::endl;
// 			// Code for handling IDLE
// 		} else if (_Connections[i]._CStatus == connectStatus::READING) {
// 			// Handle READING status
// 			std::cout << "This is 3" << std::endl;
// 			// Code for handling READING
// 		} else if (_Connections[i]._CStatus == connectStatus::REQ_ERR) {
// 			// Handle REQ_ERR status
// 			std::cout << "This is 4" << std::endl;
// 			// Code for handling REQ_ERR
// 		} else if (_Connections[i]._CStatus == connectStatus::CONNECT_CLOSED) {
// 			// Handle CONNECT_CLOSED status
// 			std::cout << "This is 5" << std::endl;
// 			// Code for handling CONNECT_CLOSED
// 		} else if (_Connections[i]._CStatus == connectStatus::DONE_READING) {
// 			// Handle DONE_READING status
// 			std::cout << "This is 6" << std::endl;
// 			// Code for handling DONE_READING
// 		} else if (_Connections[i]._CStatus == connectStatus::CGI_REQUIRED) {
// 			// Handle CGI_REQUIRED status
// 			std::cout << "This is 7" << std::endl;
// 			// Code for handling CGI_REQUIRED
// 		} else if (_Connections[i]._CStatus == connectStatus::RESPONDING) {
// 			// Handle RESPONDING status
// 			std::cout << "This is 8" << std::endl;
// 			// Code for handling RESPONDING
// 		} else if (_Connections[i]._CStatus == connectStatus::SERVER_ERR) {
// 			// Handle SERVER_ERR status
// 			std::cout << "This is 9" << std::endl;
// 			// Code for handling SERVER_ERR
// 		} else if (_Connections[i]._CStatus == connectStatus::FINISHED) {
// 			// Handle FINISHED status
// 			std::cout << "This is 10" << std::endl;
// 			// Code for handling FINISHED
// 		}
// 		}
// 	}
// }

