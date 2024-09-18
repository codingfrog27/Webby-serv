/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 16:03:09 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

class ConnectingSocket : public Socket
{
private:

public:
    ConnectingSocket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type);
    ~ConnectingSocket();

    int connectToNetwork(int socketFd, struct sockaddr_in address);
};


