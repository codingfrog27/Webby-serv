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
	if (poll.revents & POLLIN && !_request._doneReading)
	{
		if (_isServerSocket) {
			_wantsNewConnect = true;
			return;
		}
		_CStatus = _request.readRequest();
	}
	if (_CStatus == connectStatus::CONNECT_CLOSED)
		return;
	if (_CStatus == connectStatus::DONE_READING || _CStatus == connectStatus::REQ_ERR)
		_CStatus = connectStatus::RESPONDING;
	if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING)
		_CStatus = responseHandler(&_request, &_response);
	if (_CStatus == connectStatus::FINISHED)
		_CStatus = refreshIfKeepAlive();
	// else if (isTimedOut(connect._startTime, connect._TimeoutTime))
	// 	close_connect(i); // has issues??
}


connectStatus Connection::refreshIfKeepAlive() {
	std::cout << "First response FINISHED" << std::endl;
	// if (!this->_keepOpen)
			// _keepOpen = true; //move to request
	if (_request.getHeaderValue("Connection") != "keep-alive") //change to closed check
		return (connectStatus::FINISHED);
	std::cout << "connection keepopen activate" << std::endl;
	_request = Request(this->_config, this->_clientFD);
	_response = Response();
	return (connectStatus::IDLE);
}
