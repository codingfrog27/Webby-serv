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

Connection::Connection(Config *config, Server &server, int clientFD, bool isServerside)
	: _config(config), _server(server), _request(config, clientFD), _response(config), _cgi(0),
	  _isClientSocket(isServerside), _wantsNewConnect(false), _clientFD(clientFD), _keepAlive(false),
	  _startTime(getStartTime()), _IdleTimeout(setTimeout(2))
{
	if (_isClientSocket)
		_CStatus = connectStatus::IDLE;
	else
		_CStatus = connectStatus::SERV_SOCKET;
}

Connection::Connection(const Connection &rhs) :  _server(rhs._server), _request(rhs._request), _response(rhs._response)
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
		_CStatus = rhs._CStatus;
		_request = rhs._request;
		_response = rhs._response;
		_cgi = rhs._cgi;
		_wantsNewConnect = rhs._wantsNewConnect;
		_keepAlive = rhs._keepAlive;
		_startTime = rhs._startTime;
		_IdleTimeout = rhs._IdleTimeout;
	}
	return *this;
}

Connection::~Connection(void)
{
	removeCGIFromEverywhere();
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //


void	Connection::connectionAction(const pollfd &poll, Server &server)
{
	_CStatus = checkConnectStatus(poll);
	if (poll.revents & POLLIN && (_CStatus == connectStatus::IDLE || _CStatus == connectStatus::READING))
		_CStatus = _request.readRequest();
	if(_CStatus == connectStatus::CGI_REQUIRED)
		_CStatus = _cgi->CGIHandler(this, server.getCGIPollFDs(), server.getCGIMap());
	if ((poll.revents & POLLOUT) && _CStatus == connectStatus::RESPONDING)
		_CStatus = _response.responseHandler(&_request);
	if (_CStatus == connectStatus::CGI)
		_CStatus = checkCGITimeout();
}

connectStatus	Connection::checkConnectStatus(const pollfd &poll)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (poll.revents & POLLERR)
	{
		std::cout << "POLLERR flagged! " << std::endl;
		if (getsockopt(poll.fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
			std::cerr << "getsockopt failed??" << std::endl;
		if (error != 0)
			std::cerr << RED "Client closed socket mid load, closing connection" RESET << std::endl;
		return (connectStatus::CONNECT_CLOSED);
	}
	return (_CStatus);
}

bool	Connection::connectIsOkay(int fd)
{
	int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
		std::cerr << "getsockopt failed!!" << std::endl;
		return (false);
	}
	if (error == 0)
		return (true);
	std::cerr << RED "Client closed socket mid load, closing connection" RESET << std::endl;
	return (false);
}

connectStatus	Connection::checkCGITimeout()
{
	if (_cgi->CGIisTimedOut())
	{
		removeCGIFromEverywhere();
		_response.autoFillResponse("504 Gateway Timeout", "", "");
		return (connectStatus::RESPONDING);
	}
	return (connectStatus::CGI);
}

void Connection::removeCGIFromEverywhere() {
	if (_cgi.use_count() == 0)
		return; // No need to remove if the shared_ptr is already empty

	auto& pollFDs = _server.getCGIPollFDs();
	
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
	if (it != pollFDs.end()){
		close(_cgi->getFdOut());
		pollFDs.erase(it); // Erase the found element
	}
	it = std::find_if(pollFDs.begin(), pollFDs.end(), [&](const pollfd& fd) {
		return fd.fd == _cgi->getFdError(); // Match the fd value
	});
	if (it != pollFDs.end()){
		close(_cgi->getFdError());
		pollFDs.erase(it); // Erase the found element
	}
	_server.getCGIMap().erase(_cgi->getFdIn());
	_server.getCGIMap().erase(_cgi->getFdOut());
	_server.getCGIMap().erase(_cgi->getFdError());
	_cgi->killChild();
	_cgi.reset();
}
