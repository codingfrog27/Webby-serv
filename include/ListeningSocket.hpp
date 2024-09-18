/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 16:47:30 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BidingSocket.hpp"

class ListeningSocket : public BidingSocket
{
private:
    int _backlog;
    int _listening;

public:
    ListeningSocket(int t_addressFamily, int t_backlog, u_long t_host, int t_port, int t_protocol, int t_type);
    ~ListeningSocket();

    void startlistening();
};
