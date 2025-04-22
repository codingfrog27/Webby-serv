/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request_parsing.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/12 19:31:50 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/04/22 12:46:54 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

size_t	Request::parse_req_line(std::string req_line)
{
	size_t	line_end, method_end, uri_end;

	line_end = req_line.find("\r\n");
	if (line_end == 0)
	{
		while (req_line[line_end] == '\r' || req_line[line_end] == '\n')
			line_end++;
		line_end = req_line.find("\r\n");
	}
	if (line_end == std::string::npos)
		throw (ClientErrorExcept(400, "400 Bad Request"));
	method_end = req_line.find(' ');
	if (method_end == std::string::npos)
		throw (ClientErrorExcept(400, "400 Bad Request"));
	uri_end = req_line.find(' ', method_end + 1);
	if (uri_end == std::string::npos)
		throw (ClientErrorExcept(400, "400 Bad Request"));

	_method_type = which_method_type(req_line.substr(0, method_end));
	_URI = req_line.substr(method_end + 1, uri_end - method_end - 1);
	resolveFilePath();
	_http_version = http_version(&req_line[uri_end + 1]);
	return (line_end + 2);
}

void	Request::resolveFilePath()
{
	std::string resolved = _URI;

	if (resolved.find("?") != std::string::npos)
		resolved.erase(resolved.find("?"));
	else if (resolved.find("#") != std::string::npos)
		resolved.erase(resolved.find("#"));
	if (resolved.find("https://") != std::string::npos || resolved.find("http://") != std::string::npos)
		resolved.erase(0, resolved.find("//") + 2);
	if (resolved.find(_config->_host) != std::string::npos)
		resolved.erase(0, _config->_host.length());
	if (resolved.find(_config->_listen) != std::string::npos)
		resolved.erase(0, _config->_listen.length() + 1);
	
	_filePath = resolved;
	if (RouteRuleHandler() == false)
		checkIndex(_config->getIndex(), _config->getAutoindex());
	if (_aliasUsed == false)
		_filePath = _root + _filePath;
}

void	Request::parse_headers(std::string header_str)
{
	size_t						start, colon_pos;
	std::string					key, value;

	start = parse_req_line(header_str);
	for (size_t line_end = header_str.find("\r\n", start); line_end != std::string::npos;)
	{
		colon_pos = header_str.find(':', start);
		if (colon_pos == std::string::npos)
			throw (std::invalid_argument("colon missing in header"));
		key = header_str.substr(start, colon_pos - start);
		value = header_str.substr(colon_pos + 2, line_end - (colon_pos + 2));
		_headers[key] = value;
		start = line_end + 2;
		line_end = header_str.find("\r\n", start);
	}
	checkHeaders();
}

void	Request::checkHeaders()
{
	
	if (!headerExists("Host"))
		throw(ClientErrorExcept(400, "400 bad request: Host missing"));
	if (getHeaderValue("Connection") == "close")
		_keepOpen = false;
	if (_method_type ==  Http_method::GET || _method_type == Http_method::DELETE)
	{
		_doneReading = true;
		_statusStr = "";
	}
	else
		checkBodyHeaders();
}

void	Request::checkBodyHeaders()
{
	_hasBody = true;
	if (!headerExists("Content-Type"))
		throw (std::invalid_argument("415 Bad request. Unsupported Media Type"));
	if (getHeaderValue("Transfer-Encoding") == "chunked")
	{
		_dataIsChunked = true;
		reading_mode = READING_BODY_CHUNKED;
		return;
	}
	try
	{
		_contentLen = std::stoul(getHeaderValue("Content-Length"));
	}
	catch(const std::invalid_argument& e)
	{
		std::cout << e.what() << '\n';
		throw (ClientErrorExcept(413, "413 Payload too large"));
	}
	catch(const std::out_of_range& e)
	{
		std::cout << e.what() << '\n';
		throw (ClientErrorExcept(413, "413 Payload too large"));
	}
	
	if (_contentLen == 0)
		throw (ClientErrorExcept(411, "411 length required"));
	if (_contentLen > _max_body_size)
		throw (ClientErrorExcept(413, "413 Payload too large"));

	reading_mode = READING_BODY;
}

bool	Request::dechunkBody()
{
	std::string	bodyStr(_rawRequestData.begin(), _rawRequestData.end());
	size_t		bodySize = _rawRequestData.size();
	size_t		bytesParsed = 0;
	size_t		hexStrSize = 0;
	size_t		chunkSize;
	
	for (size_t rnPos = bodyStr.find("\r\n", bytesParsed); rnPos != std::string::npos;)
	{
		rnPos += 2;
		chunkSize = convertChunkSize(&bodyStr[rnPos], hexStrSize);
		if (chunkSize > bodySize - hexStrSize)
			break;
		bytesParsed += rnPos + hexStrSize + chunkSize;
		if (chunkSize == 0){
			_doneReading = true;
			break;
		}
		_reqBody += bodyStr.substr(rnPos + hexStrSize, chunkSize);
	}
	_rawRequestData.erase(_rawRequestData.begin(), (_rawRequestData.begin() + bytesParsed));
	return (_doneReading);
}


void	Request::parseBody()
{
	std::string		content_type = getHeaderValue("Content-Type");
	if (content_type.compare("application/x-www-form-urlencoded") == 0)
		parseUrlEncoded();
	_reqBody = trim(_reqBody);
	_doneReading = true;
	_statusStr = "";
}

std::string	urlDecode(const std::string &encoded);

void	Request::parseUrlEncoded()
{
	if (_reqBody.size() < 2) {
		std::cout << "url decode without body???" << std::endl;
		return;
	}
	std::istringstream stream(_reqBody.substr(2));
	std::string pair;

	while (std::getline(stream, pair, '&')) {
		size_t pos = pair.find('=');
		if (pos == std::string::npos)
			throw (ClientErrorExcept(400, "400 missing = in www-form encoded pairs"));
		_wwwFormEncodedPairs[urlDecode(pair.substr(0, pos))] = urlDecode(pair.substr(pos + 1));
	}
}