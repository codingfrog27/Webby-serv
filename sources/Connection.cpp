/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Connection.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: asimone <asimone@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/02 16:26:31 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/03 17:46:55 by asimone		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Connection.hpp"


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Connection::Connection(Config *config, int clientFD, bool isServerside): \
_config(config), _request(config, clientFD), _isServerSocket(isServerside), \
 _clientFD(clientFD), _keepOpen(false)
{
	_startTime = getStartTime();
	_TimeoutTime = intToMsecs(60000);
	if (_isServerSocket)
		_CStatus = connectStatus::SERV_SOCKET;
	else
		_CStatus = connectStatus::IDLE;
}


Connection::Connection(const Connection &rhs) : _request(rhs._request)
{
	std::cout << GREEN << "Connection: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Connection &
Connection::operator=(const Connection &rhs)
{
	std::cout << GREEN << "Connection: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_isServerSocket = rhs._isServerSocket;
		_config = rhs._config;
		_clientFD = rhs._clientFD;
		_keepOpen = rhs._keepOpen;
	}

	return (*this);
}

Connection::~Connection(void)
{
	std::cout << RED << "Connection: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

void Connection::resetRequest(Config *config, int clientFD) {
	// Explicitly call the destructor
	_request.~Request();

	// Use placement new to reconstruct the object
	new (&_request) Request(config, clientFD);
}

void Connection::resetResponse() {
	// Explicitly call the destructor
	_response.~Response();

	// Use placement new to reconstruct the object
	new (&_response) Response();
}
