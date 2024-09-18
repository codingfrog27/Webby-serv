/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 17:08:33 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BidingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"
#include "Socket.hpp"


int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	std::cout << "Starting..." << std::endl;

	std::cout << "Binding Socket..." << std::endl;
	BidingSocket bs = BidingSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
	std::cout << "Listening Socket..." << std::endl;
	ListeningSocket ls = ListeningSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
	std::cout << "Success!" << std::endl;
}

//subject mentions poll() but can use select kque or epoll as well
