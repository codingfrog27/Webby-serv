/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:32:11 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 17:52:18 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

#include <iostream>

#define MAX_CLIENT 300


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Server::Server(Config *config) : _sockets()
{
	for (size_t i = 0; i < MAX_CLIENT; i++)
	{
		_sockets.emplace_back(config);
	}
	

	std::cout << GREEN << "Server: Default constructor called" << RESET << std::endl;
}

Server::Server(const Server &rhs)
{
	std::cout << GREEN << "Server: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Server &
Server::operator=(const Server &rhs)
{
	std::cout << GREEN << "Server: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		// Perform deep copy
	}

	return (*this);
}

Server::~Server(void)
{
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //


void Server::accept_loop()
{
	int clientFD;
	for (size_t i = 0; i < _sockets.size(); i++)
	{
		clientFD = _sockets[i].createConnection();
		if (clientFD > 0)
		{
			pollfd newconnect;
			newconnect.fd = clientFD;
			//add events and revents
			pfds.push_back(newconnect); //replace w emplace?

			Connection newconnectClass(config);
			//set stuf
			connections.push_back(newconnectClass);
		}
	}
	
	
}

void	Server::close_connect(Connection closeme, int i)
{
	close(closeme._clientFD);
	close(closeme._serverFD);
	// auto it = std::find(_sockets.begin(), _sockets.end(), closeme._serverFD);
	// _sockets.erase(it);
	// _sockets.emplace(it);
	_sockets[closeme._socketIndex] = Socket(config); //does this call destructor
	pfds.erase(pfds.begin() + i);
	connections.erase(connections.begin() + i);
	// _sockets.em
	
}

void	Server::main_server_loop()
{
	
	int num_events;
	
	while (1)
		{
			accept_loop();
			//1 check for new connects
			// copy_clients_fds(connections); //2

			num_events = poll(pfds.data(), pfds.size(), 2500);
			if (num_events < 0) 
			{
				std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
				break;
			}
			if (num_events == 0) 
				std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
			for (size_t i = 0; i < pfds.size(); ++i)
			{
				if (pfds[i].revents & POLLIN) 
					connections[i]._request->main_reader(pfds[i].fd);
				if ((pfds[i].revents & POLLOUT) && connections[i].doneReading) //what if ready to post to server but not reday for response
				{
					Response newresponse(connections[i]._request);
					newresponse.autoFillResponse("replace to void");
				} 
					
					// connections[i].response->generateResponse(); //
				if (!connections[i]._keepOpen)
					close_connect(connections[i], i);
			}
			/* code */
		}
}