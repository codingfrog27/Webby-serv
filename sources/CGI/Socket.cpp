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

Socket::Socket(Config *config, const struct addrinfo *addressInfo)
 : _addrInfo(addressInfo),_setYes(1), _hostname(config->_serverName), \
 _port(config->_listen), ip_address{0}, _socketFd(0)
 {
	try
	{
		openSocket();
	}
	catch(const std::runtime_error& e)
	{
		if(_socketFd)
			close(_socketFd);
		std::cout << e.what() << "\nserver setup failed, shutting down" << std::endl;
		exit(1); //think we can exit cause  server socket creation kinda vital?
	}
	
 }

Socket::~Socket()
{
	if(_socketFd)
			close(_socketFd);
}

void	Socket::openSocket()
{
	_socketFd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	// std::cout << YELLOW "FD ==" << _socketFd << RESET << std::endl;
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set Socket to noblocking");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_KEEPALIVE, &_setYes, sizeof(_setYes)) < 0 || \
		 setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR ,&_setYes, sizeof(_setYes)) < 0)
		throw std::runtime_error(std::string("setsockopt error: ") + strerror(errno));
	if (bind(_socketFd, _addrInfo->ai_addr, _addrInfo->ai_addrlen) < 0)
		throw std::runtime_error(std::string("Bind errorr: ") + strerror(errno));
	if (listen(_socketFd, 20) < 0) //SET TO CONFIG VALUE
		throw std::runtime_error(std::string("Listen Error: ") + strerror(errno));
	int listening = 0;
	socklen_t len = sizeof(listening);
	getsockopt(_socketFd, SOL_SOCKET, SO_ACCEPTCONN, &listening, &len);
	if (!listening) {
		std::cerr << "ERROR: FD is not a listening socket!" << std::endl;
	}
	// std::cout << CYAN "socket opened" << std::endl;
	freeaddrinfo(const_cast<struct addrinfo *>(_addrInfo));
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

	new_socket = accept(_socketFd, (struct sockaddr *)&_address, &addrlen);
		if (new_socket >= 0)
			inet_ntop(_address.ss_family, get_in_addr((struct sockaddr *)&_address), ip_address, sizeof(ip_address)); //stores address in _address
		else if (errno == EAGAIN || errno == EWOULDBLOCK)
			std::cout << CYAN << "No connections available, retrying..." << RESET << std::endl;
		else
			throw std::runtime_error(std::string("Accept failed with error: ") \
			 + strerror(errno) + "FD ==" + std::to_string(_socketFd));
	return (new_socket);
}