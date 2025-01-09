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
		_CStatus = rhs._CStatus;
		_request = rhs._request;
		_response = rhs._response;
		_startTime = rhs._startTime;
		_TimeoutTime = rhs._TimeoutTime;
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
	int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(poll.fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
	{
		std::cout << "getsockopt failed" << std::endl;
		return;

	}
	else if (error != 0) {
		std::cout << RED "Socket error: " << strerror(error) << std::endl;
		// NicePrint::promptEnter();
	}
	else if (poll.revents & POLLHUP) {
		std::cout << "Client disconnected (POLLHUP)" << std::endl;
		return ;
	} 
	else if (poll.revents & POLLERR) {
		std::cout << "Socket error (POLLERR)" << std::endl;
		return ;
	}


	if (poll.revents & POLLIN && !_request._doneReading)
	{
		if (_isServerSocket) {
			_wantsNewConnect = true;
			return;
		}
		_CStatus = _request.readRequest();
	}
	if (_CStatus == connectStatus::CONNECT_CLOSED)
	{
		return;
	}
	else if (_CStatus == connectStatus::DONE_READING || _CStatus == connectStatus::REQ_ERR)
		_CStatus = connectStatus::RESPONDING;
	else if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING)
	{
		_CStatus = responseHandler(&_request, &_response);
	}
	else if (_CStatus == connectStatus::FINISHED)
	{
		_CStatus = refreshIfKeepAlive();

	}
	// else if (isTimedOut(connect._startTime, connect._TimeoutTime))
	// 	close_connect(i); // has issues??
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
