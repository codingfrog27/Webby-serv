/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/03 14:32:11 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/07 18:32:31 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "everything.hpp"

#include <iostream>

#define MAX_CLIENT 300


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Server::Server(Config *config) : _config(config), _sockets(), _max_clients(config->_maxConnects), _addrInfo{0}
{	
	try
	{
		setupAddrInfo();
		for (size_t i = 0; i < MAX_CLIENT; i++)
		{
			_sockets.emplace_back(config, _addrInfo);
			pfds.emplace_back(pollfd{_sockets[i]._socketFd, POLLIN | POLLOUT | POLLERR | POLLHUP, 0});
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	std::cout << GREEN << "Server: Default constructor called" << RESET << std::endl;
}

void	Server::setupAddrInfo()
{
	addrinfo hints;
	int status;
	hints.ai_family = AF_UNSPEC; //Specifies the address family. AF_UNSPEC = don't care IPv4 or IPv6
	hints.ai_flags = AI_PASSIVE; //Provides additional options (AI_PASSIVE for binding to all network interfaces)
	hints.ai_protocol = IPPROTO_TCP; //Specifies the protocol
	hints.ai_socktype = SOCK_STREAM; //Specifies the socket type (SOCK_STREAM for TCP)

	status = getaddrinfo(_config->_serverName.c_str(), _config->_serverPort.c_str(), &hints, &_addrInfo); 
	if (status != 0)
		throw std::runtime_error(std::string("getaddrinfo error: ") + gai_strerror(status));

}

void Server::accept_loop(int i)
{
	int clientFD;
		clientFD = _sockets[i].createConnection();
		if (clientFD > 0)
			connections.emplace_back(_config, clientFD);
}

Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //



void	Server::close_connect(Connection closeme, int i)
{
	close(closeme._clientFD);
	// close(closeme._serverFD);
	// auto it = std::find(_sockets.begin(), _sockets.end(), closeme._serverFD);
	// _sockets.erase(it);
	// _sockets.emplace(it);
	_sockets[closeme._socketIndex] = Socket(_config, _addrInfo); //does this call destructor
	pfds.erase(pfds.begin() + i);
	connections.erase(connections.begin() + i);
	// _sockets.em
	
}

void	Server::main_server_loop()
{
	
	int num_events;
	
	while (1)
		{
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
			for (size_t i = 0; i < num_events; ++i)
			{
				if (pfds[i].revents & POLLIN) //take other events
				{

					accept_loop(i);
					connections[i]._request->main_reader(pfds[i].fd);
				} 
				if ((pfds[i].revents & POLLOUT) && connections[i].doneReading) //what if ready to post to server but not reday for response
					responseHandler(connections[i]._request);
					// responseHandler(connections[i]._request, connections[i]._clientFD);
					
					// connections[i].response->generateResponse(); //
				if (!connections[i]._keepOpen)
					close_connect(connections[i], i);
			}
			/* code */
		}
}

// Server::Server(const Server &rhs) :  _max_clients(rhs._max_clients)
// {
// 	std::cout << GREEN << "Server: Copy constructor called" << RESET << std::endl;

// 	*this = rhs;
// }

// Server &
// Server::operator=(const Server &rhs)
// {
// 	std::cout << GREEN << "Server: Assignment operator called" << RESET << std::endl;

// 	if (this != &rhs)
// 	{
// 		// Perform deep copy
// 	}

// 	return (*this);
// }
