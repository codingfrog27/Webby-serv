/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   newSocket.cpp									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/08 11:36:53 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/08 11:37:05 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "socket.hpp"

Socket::Socket(Config *config)
 : _setYes(1), _hostname(config->_serverName), _port(config->_serverPort), _socketFd(0), ip_address{0}
 {
	try
	{
		openSocket();
	}
	catch(const std::runtime_error& e)
	{
		if(_socketFd)
			close(_socketFd);
		std::cerr << e.what() << '\n';
		//think we can exit cause socket creation kinda vital?
		exit(1);
	}
	
 }

Socket::~Socket()
{
	if(_socketFd)
			close(_socketFd);
	// std::cout << RED << "Destructor socket has been called." << RESET << std::endl;
}

void	Socket::openSocket()
{
	_socketFd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set Socket to noblocking");
	if (setsockopt(_socketFd, SOL_SOCKET,SO_REUSEADDR ,&_setYes, sizeof(_setYes)) == -1 || \
		setsockopt(_socketFd, SOL_SOCKET, SO_REUSEPORT, &_setYes, sizeof(_setYes)) == -1)
		throw std::runtime_error(std::string("setsockopt error: ") + strerror(errno));
	if (bind(_socketFd, _addrInfo->ai_addr, _addrInfo->ai_addrlen) == -1)
		throw std::runtime_error(std::string("Bind errorr: ") + strerror(errno));
}

void *Socket::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int	Socket::createConnection()
{   
	socklen_t   addrlen = sizeof(_address);
	int		 new_socket; 
	
	//This function configures a socket to listen for incoming connection requests from clients. 
	//After binding a socket to an address and port, we use listen() to indicate that the socket is ready to accept incoming connections.

	std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
	//This function is called to accept an incoming connection request on a socket that has been set up to listen for connections. 
	//When a client attempts to connect to the server, accept() creates a new socket for that connection and establishes the communication channel.
	new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
		if (new_socket >= 0)
			inet_ntop(_address.ss_family, get_in_addr((struct sockaddr *)&_address), ip_address, sizeof(ip_address));
		// {
		// 	// std::cout << GREEN << "New connection accepted" << RESET << std::endl;
		// 	// std::cout << CYAN << "server: got connection from " << ip_address << RESET << std::endl;
		// }
		else if (errno == EAGAIN || errno == EWOULDBLOCK)
			std::cout << CYAN << "No connections available, retrying..." << RESET << std::endl;
		else
			throw std::runtime_error(std::string("Accept failed with error: ") + strerror(errno));
	return (new_socket);
}
