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

Server::Server(Config *config) : _config(config), _max_clients(config->_maxConnects), _addrInfo{0}
{	
	try
	{
		setupAddrInfo();
		_serverSocket = new Socket(config, _addrInfo); //move adrresinfo back to so
		_pollFDs.emplace_back(pollfd{_serverSocket->_socketFd, POLLIN | POLLERR,  0});
		//main loop?
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


Server::~Server(void)
{
	freeaddrinfo(_addrInfo);
	std::cout << RED << "Server: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //



void	Server::close_connect(int i)
{
	close(_Connections[i]._clientFD);
	_Connections.erase(_Connections.begin() + i);
	_pollFDs.erase(_pollFDs.begin() + i);
}

static int s_counter;

void	Server::main_server_loop()
{
	int	size = _pollFDs.size();
	int c_i;
	poll(_pollFDs.data(), size, -1); //set diff timeout and mb handle error
	if (s_counter == 500)
		std::cout << "DEAR LORDDD\n" << std::endl;

	if (_pollFDs[0].revents & POLLIN)
	{
		std::cout << "DEAR LORDDD\n" << std::endl;
		acceptNewConnects();
	}
	for (size_t i = 1; i < size; ++i)
	{		
		c_i = i - 1;
		if (_pollFDs[i].revents & POLLIN)
			_Connections[i]._request.readRequest();
		if ((_pollFDs[i].revents & POLLOUT) && _Connections[i]._doneReading) //getter?
		{
			responseHandler(&_Connections[i]._request);
			if (_Connections[i]._keepOpen)
				_Connections[i]._request = Request(_Connections[i]._clientFD);
			else
			close_connect(i);
		}
	}
	s_counter++;
}



void Server::acceptNewConnects()
{
	for (int clientFD = accept(_pollFDs[0].fd, nullptr, nullptr); clientFD > 0;)
	{
		_pollFDs.emplace_back(pollfd{clientFD, POLLIN, 0});
		_Connections.emplace_back(Connection(_config, clientFD));
	}
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
// }

	// _Requests[i].readRequest();
		// if (num_events < 0) 
		// {
		// 	std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
		// 	break;
		// }

// void	Server::main_server_loop()
// {
	
// 	std::unordered_map<int, Request>\
// 	::iterator	it;
// 	int			num_events;
	
// 	num_events = poll(_pollFDs.data(), _pollFDs.size(), -1); //handle poll error
// 	if (num_events == 0) 
// 		std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
// 	else
// 		std::cout << "YIPPIE" << std::endl;
// 	for (size_t i = 0; i < MAX_CLIENT; ++i)
// 	{
// 		if (_pollFDs[i].revents & POLLIN) //take other events
// 		{
// 			it = _Requests.find(i);
// 			if (it == _Requests.end())
// 			{
// 				auto result = _Requests.emplace(i, acceptNewConnects());
// 				// catch construct error?
// 				it = result.first;
// 			}
// 			it->second.readRequest();
// 		}
// 		if (_pollFDs[i].revents & POLLOUT)
// 		{
// 			it = _Requests.find(i);
// 			if (it != _Requests.end() && it->second._doneReading) //can i put this if into the above condition?
// 			{
// 				responseHandler(&it->second);
// 				if (!it->second._keepOpen)
// 					close_connect(it->second, i);
// 			}
// 		}
// 		// else
// 			// refresh req object only
// 	}
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
