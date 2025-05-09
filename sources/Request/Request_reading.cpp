/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   HttpRequest_reading.cpp							:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/06 17:22:52 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/04 17:58:36 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Request.hpp"
#include "Connection.hpp"
#include <signal.h>

connectStatus	Request::readRequest()
{
	try
	{
		if (_statusStr.empty() || _statusStr == "0 Not started yet")
			_statusStr = "102 Processing";

		readSocket(0);

		if (isTimedOut(this->_startTime, this->_timeoutTime))
			throw ClientErrorExcept(408, "408 Request Timeout");
		if (!_rnrnFound && headerEndFound())
			parse_headers(_unsortedHeaders);
		if (_hasBody && bodyIsRead())
			parseBody();
		if (_doneReading)
		{
			if (_cgiRequired == true || isCGIrequired() == true)
				return (connectStatus::CGI_REQUIRED);
			return (connectStatus::RESPONDING);
		}
		return (connectStatus::READING);
	}
	catch(ClientErrorExcept &e)
	{
		// std::cerr << RED "Client error: " << e.what() << RESET << std::endl;
		_statusStr = e._errorMsg;
		_statusCode = e._statusCode;
		return (connectStatus::RESPONDING);
	}
	catch (ConnectionClosedExcep &e)
	{
		std::cout << "Client closed connection" << std::endl;
		return (connectStatus::CONNECT_CLOSED);
	}
	catch(const std::ios_base::failure &e)
	{
		std::cout << e.what() << std::endl;
		_statusCode = 500;
		_statusStr = std::to_string(_statusCode) + ' ' + e.what();
		return (connectStatus::CONNECT_CLOSED);
	}
	catch(std::invalid_argument &e)
	{
		std::cout << e.what() << std::endl;
		_statusCode = 400;
		_statusStr = std::to_string(_statusCode) + e.what();
		return (connectStatus::RESPONDING);
	}
}

int	Request::readSocket(int size)
{
	if (!size)
		size = BUFFER_SIZE;
	char buffer[size];
	if (!Connection::connectIsOkay(_clientFD))
		throw (ConnectionClosedExcep(_clientFD));
	int	bytes_read = recv(_clientFD, buffer, size, MSG_DONTWAIT);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			throw(ConnectionClosedExcep(_clientFD));
		else
			throw (std::ios_base::failure(" reading fail when reading from client socket"));
	}
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	if (_rnrnFound)
		body_bytes_read += bytes_read;
	return (bytes_read);
}

bool	Request::headerEndFound()
{
	static const std::vector< unsigned char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<unsigned char>::iterator it = std::search(_rawRequestData.begin(),\
	_rawRequestData.end(),body_delim.begin(), body_delim.end());
	if (it == _rawRequestData.end()) {
		reading_mode = READING_HEADERS;
		return (false);
	}
	_rnrnFound = true;
	_unsortedHeaders = std::string(_rawRequestData.begin(), it + 2);
	_rawRequestData.erase(_rawRequestData.begin(), it + 2);
	body_bytes_read = _rawRequestData.size();
	return (true);
}

bool	Request::bodyIsRead()
{
	if (body_bytes_read > _max_body_size)
			throw (ClientErrorExcept(413, "413 Payload too large"));
	if (_dataIsChunked && dechunkBody() == true){
		std::cout << BOLD "data is chunked in body is read" RESET << std::endl;
		return(true);
	}
	else if (body_bytes_read >= _contentLen)
	{
		_doneReading = true;
		reading_mode = FINISHED;
		_reqBody = std::string(_rawRequestData.begin(), (_rawRequestData.begin() + _contentLen + 2));
		_rawRequestData.erase(_rawRequestData.begin(), _rawRequestData.begin() + _contentLen + 2);
		return (true);
	}
	return (_doneReading);
}

bool	Request::isCGIrequired()
{
	if (this->_filePath.rfind(".py") == this->_filePath.length() - 3 || this->_filePath.rfind(".js") == this->_filePath.length() - 3)
		return true;
	if (this->_headers["Content-Type"].find("multi-part/form-data") != std::string::npos)
		return true;
	if (this->_headers["Content-Type"].find("application/x-www-form-urlencoded") != std::string::npos)
		return true;
	return false;
}
