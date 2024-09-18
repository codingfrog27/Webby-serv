/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindingSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 17:08:05 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BidingSocket.hpp"

BidingSocket::BidingSocket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type) : Socket (t_addressFamily, t_host, t_port, t_protocol, t_type)
{
    // setConnection(connectToNetwork(_SocketFd, _address));
    _connection = connectToNetwork(_SocketFd, _address);
    test_connection(_connection);

    std::cout << GREEN << "Parameterized Constructor BidingSocket has been called." << RESET << std::endl;
}

BidingSocket::~BidingSocket()
{
    std::cout << RED << "Default Destructor BidingSocket has been called." << RESET << std::endl;
}

int BidingSocket::connectToNetwork(int socketFd, struct sockaddr_in address)
{
    return (bind(socketFd, (struct sockaddr *)&address, sizeof(address)));
}