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
_wantsNewConnect(false), _clientFD(clientFD), _keepOpen(false)
{
	_startTime = getStartTime();
	_IdleTimeout = intToMsecs(600);
	// _IdleTimeout = intToMsecs(config->_timeout);
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
		_CStatus = rhs._CStatus;
		_request = rhs._request;
		_response = rhs._response;
		_startTime = rhs._startTime;
		_IdleTimeout = rhs._IdleTimeout;
		_wantsNewConnect = rhs._wantsNewConnect;
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

void	Connection::connectionAction(const pollfd &poll)
{
	if (_isServerSocket)
	{
		if (poll.revents & POLLIN)
			_wantsNewConnect = true;
		return;
	}
	checkConnectErrors(poll);
	if (poll.revents & POLLIN && \
	(_CStatus == connectStatus::IDLE || _CStatus == connectStatus::READING)) //double
		_CStatus = _request.readRequest();
	if (_CStatus == connectStatus::CONNECT_CLOSED)
		return;
	//done reading and req error could both just be responding to make things easier
	if (_CStatus == connectStatus::DONE_READING || _CStatus == connectStatus::REQ_ERR)
		_CStatus = connectStatus::RESPONDING;
	if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING)
		_CStatus = responseHandler(&_request, &_response);
	if (_CStatus == connectStatus::FINISHED)
		_CStatus = refreshIfKeepAlive();
	// else if (isTimedOut(connect._startTime, connect._IdleTimeout))
	// 	close_connect(i); // has issues??
}

void	Connection::checkConnectErrors(const pollfd &poll)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (poll.revents & POLLHUP) {
		std::cerr << "Client disconnected (POLLHUP)" << std::endl;
	} 
	if (poll.revents & POLLERR) {
		std::cerr << "Socket error (POLLERR)" << std::endl;
		if (getsockopt(poll.fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			std::cerr << "getsockopt failed" << std::endl;
		if (error != 0)
			std::cerr << RED "Socket error: " << strerror(error) << RESET << std::endl;
			// NicePrint::promptEnter();
		_CStatus = connectStatus::CONNECT_CLOSED;
	}
}


connectStatus Connection::refreshIfKeepAlive()
{
	std::cerr << "First response FINISHED" << std::endl;
	// if (!this->_keepOpen)
			// _keepOpen = true; //move to request
	if (_request.getHeaderValue("Connection") != "keep-alive")
	{
		std::cout << "close meee" << std::endl;
		return (connectStatus::FINISHED);
	} //change to closed check
	std::cout << "connection keep open activate" << std::endl;
	_request = Request(this->_config, this->_clientFD);
	_response = Response();
	return (connectStatus::IDLE);
}
