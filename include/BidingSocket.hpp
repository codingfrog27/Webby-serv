/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BidingSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 18:41:56 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 16:43:47 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Socket.hpp"

class BidingSocket : public Socket
{
private:
    
public:
    BidingSocket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type);
    ~BidingSocket();
    
    int connectToNetwork(int socketFd, struct sockaddr_in address);
};

