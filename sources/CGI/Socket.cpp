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
 _port(config->_serverPort), _socketFd(0), ip_address{0}
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

	new_socket = accept(_socketFd, (struct sockaddr *)&_address, &addrlen);
		if (new_socket >= 0)
			inet_ntop(_address.ss_family, get_in_addr((struct sockaddr *)&_address), ip_address, sizeof(ip_address)); //stores address in _address
		else if (errno == EAGAIN || errno == EWOULDBLOCK)
			std::cout << CYAN << "No connections available, retrying..." << RESET << std::endl;
		else
			throw std::runtime_error(std::string("Accept failed with error: ") + strerror(errno) + "FD ==" + std::to_string(_socketFd));
	return (new_socket);
}

	// std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
		// {
		// 	// std::cout << GREEN << "New connection accepted" << RESET << std::endl;
		// 	// std::cout << CYAN << "server: got connection from " << ip_address << RESET << std::endl;
		// }