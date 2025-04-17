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
#include "CGI.hpp"
#include "Server.hpp"


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Connection::Connection(Config *config, int clientFD, bool isServerside): \
_config(config), _request(config, clientFD), _response(config), _cgi(0), \
_isClientSocket(isServerside), _wantsNewConnect(false), _clientFD(clientFD), _keepOpen(false)
{
	_startTime = getStartTime();
	_IdleTimeout = setTimeout(2);
	// _IdleTimeout = setTimeout(config->_timeout);
	if (_isClientSocket)
		_CStatus = connectStatus::IDLE;
	else
		_CStatus = connectStatus::SERV_SOCKET;
}


Connection::Connection(const Connection &rhs) : _request(rhs._request), _response(rhs._response)
{
	*this = rhs;
}

Connection &
Connection::operator=(const Connection &rhs)
{

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
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //


void	Connection::connectionAction(const pollfd &poll, Server &server)
{
	_CStatus = checkConnectStatus(poll);
	if (poll.revents & POLLIN && (_CStatus == connectStatus::IDLE || \
									_CStatus == connectStatus::READING))
		_CStatus = _request.readRequest();
	if(_CStatus == connectStatus::CGI_REQUIRED){
		_CStatus = _cgi->CGIHandler(this, server.getCGIPollFDs(), server.getCGIMap());
		std::cout << MAGENTA "CGI PollFD vector size in connectionAction: " << server.getCGIPollFDs().size() << RESET << std::endl;
	}
	if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING){
		_CStatus = _response.responseHandler(&_request);
	}
	if (_CStatus == connectStatus::FINISHED)
		_CStatus = refreshIfKeepAlive();
}

connectStatus	Connection::checkConnectStatus(const pollfd &poll)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (poll.revents & POLLERR)
	{
		std::cout << "Socket error (POLLERR)" << std::endl;
		if (getsockopt(poll.fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			std::cout << "getsockopt failed" << std::endl;
		if (error != 0)
			std::cout << RED "Socket error: " << strerror(error) << RESET << std::endl;
		return (connectStatus::CONNECT_CLOSED);
	}
	// else if (isTimedOut(_startTime, _IdleTimeout) || poll.revents & POLLHUP)
	// 	return (connectStatus::CONNECT_CLOSED);
	return (_CStatus);
}

bool	Connection::connectIsOkay(int fd)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
		std::cout << "getsockopt failed" << std::endl;
		return (false);
	}
	if (error == 0)
		return (true);
	std::cerr << RED "Socket error: " << strerror(error) << RESET << std::endl;
	return (false);
}

void Connection::removeCGIFromEverywhere(Server& server) {
	auto& pollFDs = server.getCGIPollFDs();
	auto it = std::find_if(pollFDs.begin(), pollFDs.end(), [&](const pollfd& fd) {
		return fd.fd == _cgi->getFdIn(); // Match the fd value
	});
	if (it != pollFDs.end()) {
		close(_cgi->getFdIn());
		pollFDs.erase(it); // Erase the found element
	}
	it = std::find_if(pollFDs.begin(), pollFDs.end(), [&](const pollfd& fd) {
		return fd.fd == _cgi->getFdOut(); // Match the fd value
	});
	if (it != pollFDs.end()) {
		close(_cgi->getFdOut());
		pollFDs.erase(it); // Erase the found element
	}
	it = std::find_if(pollFDs.begin(), pollFDs.end(), [&](const pollfd& fd) {
		return fd.fd == _cgi->getFdError(); // Match the fd value
	});
	if (it != pollFDs.end()) {
		close(_cgi->getFdError());
		pollFDs.erase(it); // Erase the found element
	}
	server.getCGIMap().erase(_cgi->getFdIn());
	server.getCGIMap().erase(_cgi->getFdOut());
	server.getCGIMap().erase(_cgi->getFdError());
	_cgi.reset();
}

connectStatus Connection::refreshIfKeepAlive()
{
	if (_request.getHeaderValue("Connection") != "keep-alive" || !_request._statusStr.empty())
	{
		std::cout << "close meee" << std::endl;
		// shutdown(_clientFD, SHUT_RD);
		// shutdown(_clientFD, SHUT_WR);
		return (connectStatus::FINISHED);
	}
	std::cout << CYAN "connection keep open activate on FD: " << YELLOW << this->_clientFD << RESET << std::endl;
	_request = Request(this->_config, this->_clientFD);
	_response = Response(this->_config);
	return (connectStatus::IDLE);
}
