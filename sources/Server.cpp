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

Request Server::accept_connection(int i)
{
	int clientFD;
		clientFD = _sockets[i].createConnection();
			return (Request(clientFD));
		// if (clientFD > 0)
		//else handle exception
}

Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //



void	Server::close_connect(Request closeme, int i)
{
	close(closeme._clientFD);
	_Requests.erase(i); //might need to change if i need data from last request
}

void	Server::main_server_loop()
{
	
	std::unordered_map<int, Request>\
	::iterator	it;
	int			num_events;
	
	num_events = poll(pfds.data(), pfds.size(), 2500); //handle poll error
	if (num_events == 0) 
		std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
	for (size_t i = 0; i < num_events; ++i)
	{
		if (pfds[i].revents & POLLIN) //take other events
		{
			it = _Requests.find(i);
			if (it == _Requests.end())
			{
				auto result = _Requests.emplace(i, accept_connection(i));
				// catch construct error?
				it = result.first;
			}
			it->second.main_reader();
		}
		if ((pfds[i].revents & POLLOUT) && _Requests[i]._doneReading)
			responseHandler(&_Requests[i]);
		if (!_Requests[i]._keepOpen)
			close_connect(_Requests[i], i);
		// else
			// refresh req object only
	}
}


void	Server::epollLoop()
{
	
}


// void	Server::handleEvents()
// {
// 	auto it = _Requests.find(i);  // Use find() to check if the key exists

// 	if (it != _Requests.end()) {
//     if ((pfds[i].revents & POLLOUT) && it->second._doneReading)
//         responseHandler(&(it->second));  // Access the value via iterator
    
//     if (!it->second._keepOpen)
//         close_connect(it->second, i);
// }
}

	// _Requests[i].main_reader();
		// if (num_events < 0) 
		// {
		// 	std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
		// 	break;
		// }














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
