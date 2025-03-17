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
_config(config), _request(config, clientFD), _isClientSocket(isServerside), \
_wantsNewConnect(false), _clientFD(clientFD), _keepOpen(false)
{
	_startTime = getStartTime();
	_IdleTimeout = setTimeout(2);
	// _IdleTimeout = setTimeout(config->_timeout);
	if (_isClientSocket)
		_CStatus = connectStatus::IDLE;
	else
		_CStatus = connectStatus::SERV_SOCKET;
}


Connection::Connection(const Connection &rhs) : _request(rhs._request)
{
	// std::cout << GREEN << "Connection: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Connection &
Connection::operator=(const Connection &rhs)
{
	// std::cout << GREEN << "Connection: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_isClientSocket = rhs._isClientSocket;
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
	// std::cout << RED << "Connection: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

	// if (_CStatus == connectStatus::CONNECT_CLOSED)
	// 	return;
	//done reading and req error could both just be responding to make things easier
	// if (_CStatus == connectStatus::DONE_READING || _CStatus == connectStatus::REQ_ERR)
	// 	_CStatus = connectStatus::RESPONDING;
	// if (_isClientSocket)
	// {
	// 	if (poll.revents & POLLIN)
	// 		_wantsNewConnect = true;
	// 	return;
	// }
void	Connection::connAction(const pollfd &poll, Server &server)
{
	_CStatus = checkConnectStatus(poll);
	if (poll.revents & POLLIN && (_CStatus == connectStatus::IDLE || \
									_CStatus == connectStatus::READING))
		_CStatus = _request.readRequest();

	if (_CStatus == connectStatus::CGI_REQUIRED)
		(void)server;
	if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING)
		_CStatus = responseHandler(&_request, &_response);

	if (_CStatus == connectStatus::FINISHED)
		_CStatus = refreshIfKeepAlive();
}

//print to info log and or error log file
	// if (poll.revents & POLLHUP) {
	// 	std::cout << "Client disconnected (POLLHUP)" << std::endl;
	// } 
connectStatus	Connection::checkConnectStatus(const pollfd &poll)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (poll.revents & POLLERR) {
		std::cout << "Socket error (POLLERR)" << std::endl;
		if (getsockopt(poll.fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			std::cout << "getsockopt failed" << std::endl;
		if (error != 0)
			std::cout << RED "Socket error: " << strerror(error) << RESET << std::endl;
			// NicePrint::promptEnter();
		return (connectStatus::CONNECT_CLOSED); 
	}
	else if (isTimedOut(_startTime, _IdleTimeout) || poll.revents & POLLHUP)
		return (connectStatus::CONNECT_CLOSED); 
	return (_CStatus);
}


connectStatus Connection::refreshIfKeepAlive()
{
	// std::cout << "First response FINISHED" << std::endl;
	// if (!this->_keepOpen)
			// _keepOpen = true; //move to request
	if (_request.getHeaderValue("Connection") != "keep-alive")
		return (connectStatus::FINISHED);
	std::cout << "connection stays open" << std::endl;
	_request = Request(this->_config, this->_clientFD);
	_response = Response();
	return (connectStatus::IDLE);
}
