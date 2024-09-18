/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 17:49:50 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type)
{
	//Identify (name) a socket
	_address.sin_family = t_addressFamily;
	_address.sin_port = htons(t_port);			//Fill the struct sockaddr_in 
	_address.sin_addr.s_addr = htonl(t_host);

	//Create a socket
	_SocketFd = socket(t_addressFamily, t_type, t_protocol);
	test_connection(_SocketFd);
	//Establish network connection
	// _connection = connectToNetwork(_SocketFd, _address);
	// test_connection(_connection);

	std::cout << GREEN << "Parameterized Constructor Socket has been called." << RESET << std::endl;
}

Socket::~Socket()
{
	std::cout << RED << "Default Destructor Socket has been called." << RESET << std::endl;
}

void Socket::test_connection(int test)
{
	if (test < 0)
	{
		std::cerr << RED << "Failed to connect..." << RESET << std::endl;
		exit (EXIT_FAILURE);
	}
}

void Socket::setConnection(int connect)
{
	_connection = connect;
}

const int& Socket::getconnection()
{
	return _connection;
}

const struct sockaddr_in& Socket::getaddress()
{
	return _address;
}