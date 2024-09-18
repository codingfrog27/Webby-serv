/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectingSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 17:01:50 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectingSocket.hpp"

ConnectingSocket::ConnectingSocket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type) : Socket (t_addressFamily, t_host, t_port, t_protocol, t_type)
{
    // setConnection(connectToNetwork(_SocketFd, _address));
    _connection = connectToNetwork(_SocketFd, _address);
    test_connection(_connection);

    std::cout << GREEN << "Parameterized Constructor ConnectingSocket has been called." << RESET << std::endl;
}

ConnectingSocket::~ConnectingSocket()
{
    std::cout << RED << "Parameterized Constructor ConnectingSocket has been called." << RESET << std::endl;
}

int ConnectingSocket::connectToNetwork(int socketFd, struct sockaddr_in address)
{
    return(connect(socketFd, (struct sockaddr *)&address, sizeof(address)));
}