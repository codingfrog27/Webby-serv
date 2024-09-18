/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 17:52:09 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int t_addressFamily, int t_backlog, u_long t_host, int t_port, int t_protocol, int t_type) : BidingSocket(t_addressFamily, t_host, t_port, t_protocol, t_type)
{
    this->_backlog = t_backlog;
    
    startlistening();
    test_connection(_listening);

    std::cout << GREEN << "Parameterized Constructor ListeningSocket has been called." << RESET << std::endl;
}

ListeningSocket::~ListeningSocket()
{
    std::cout << GREEN << "Parameterized Constructor ListeningSocket has been called." << RESET << std::endl;
}

void ListeningSocket::startlistening()
{
    std::cout << _connection << std::endl;
    this->_listening = listen(_connection, _backlog);
}