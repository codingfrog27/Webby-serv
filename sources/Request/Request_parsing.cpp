/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:31:50 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/01/09 14:58:31 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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
		throw (std::invalid_argument("no CRLF found"));

	method_end = req_line.find(' ');
	if (method_end == std::string::npos)
		throw (std::invalid_argument("no space found after method"));
	uri_end = req_line.find(' ', method_end + 1); //could check on single space
	if (uri_end == std::string::npos)
		throw (std::invalid_argument("no space found after uri"));

	_method_type = which_method_type(req_line.substr(0, method_end));
	_URI = req_line.substr(method_end + 2, uri_end - method_end - 1); //temp + 2??
	resolveFilePath();
	std::string file_path = this->_filePath;
	checkForRedirect(file_path);
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
	if (resolved.find("https://") != std::string::npos || resolved.find("http://") != std::string::npos) //do we wanna handle https?
		resolved.erase(0, resolved.find("//") + 2);
	if (resolved.find(_config->_host) != std::string::npos)
		resolved.erase(0, _config->_host.length());
	if (resolved.find(_config->_listen) != std::string::npos)
		resolved.erase(0, _config->_listen.length() + 1);
	if (resolved.front() == '/')
		resolved.erase(0, 1);
	_filePath = trim(_config->_rootDir) + trim(resolved);
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
		value = header_str.substr(colon_pos + 2, line_end - (colon_pos + 2)); //check for empty values?
		_headers[key] = value;
		start = line_end + 2;
		line_end = header_str.find("\r\n", start);
	}
	checkHeaders();
}

void	Request::checkHeaders()
{
	if (!headerExists("Host"))
		throw(std::invalid_argument("400 bad request: Host missing"));
	if (getHeaderValue("Connection") == "close")
		_keepOpen = false;
	if (_method_type == Http_method::GET)
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
	if (!headerExists("Content-Type")) //or if type not supported
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
		reading_mode = READING_BODY;
	}
	catch(const std::invalid_argument& e)
	{
		std::cerr << e.what() << '\n';
		throw (ClientErrorExcept(413, "413 Payload too large"));
	}
	catch(const std::out_of_range& e)
	{
		std::cerr << e.what() << '\n';
		throw (ClientErrorExcept(413, "413 Payload too large"));
	}
	if (_contentLen == 0)
		throw (ClientErrorExcept(411, "411 length required"));
	if (_contentLen > _max_body_size)
		throw (ClientErrorExcept(413, "413 Payload too large"));
}

bool	Request::dechunkBody()
{
	std::string	bodyStr(_rawRequestData.begin(), _rawRequestData.end());
	size_t		bodySize = _rawRequestData.size();
	size_t		bytesParsed = 0;
	size_t		hexStrSize = 0;
	size_t		chunkSize;
	//assumes the starting rnrn of body is still there but also only last one
	for (size_t rnPos = bodyStr.find("\r\n", bytesParsed); rnPos != std::string::npos;)
	{
		rnPos += 2;
		chunkSize = convertChunkSize(&bodyStr[rnPos], hexStrSize);
		if (chunkSize > bodySize - hexStrSize) //means
			break;
		bytesParsed += rnPos + hexStrSize + chunkSize;
		if (chunkSize == 0){
			_doneReading = true;
			break;
		}
		_reqBody += bodyStr.substr(rnPos + hexStrSize, chunkSize);
	}
	_rawRequestData.erase(_rawRequestData.begin(), (_rawRequestData.begin() + bytesParsed));
	//do i have to remove 2 more if we're at the end? if so can make chunksize 2 :)
	return (_doneReading);
}


void	Request::parseBody()
{
	std::string		content_type = getHeaderValue("Content-Type");
	if(content_type.compare("multipart/form-data; boundary=") == 0)
		parseFormData(content_type);
	else if (content_type.compare("application/x-www-form-urlencoded") == 0)
		parseUrlEncoded();
	_reqBody = trim(_reqBody);
	_doneReading = true;
	_statusStr = "";
}

std::string	urlDecode(const std::string &encoded);

void	Request::parseUrlEncoded()
{
	std::istringstream stream(_reqBody.substr(2));
	std::string pair;

	while (std::getline(stream, pair, '&')) {
		size_t pos = pair.find('=');
		if (pos == std::string::npos)
			throw (ClientErrorExcept(400, "400, missing = in www-form encoded pairs"));
		_wwwFormEncodedPairs[urlDecode(pair.substr(0, pos))] = urlDecode(pair.substr(pos + 1));
	}
}


void	Request::parseFormData(std::string &content_type){
	//assuming its there cause of header check
	size_t nextboundary;
		if (content_type.size() < 31) //meaning multiform without boundery!
			throw(ClientErrorExcept(400, "400, Bad Request, empty boundary parameter"));
	std::string delimiter = "--" + content_type.substr(31);
	for (size_t i = _reqBody.find(delimiter); i != std::string::npos; i = nextboundary)
	{
		if (_reqBody.compare(i, 2, "--") == 0)
			break;

		//make new form object
		//object.headers = parse_headers(&_reqBody[i + delimiter.size()]);


		//change parse headers to take table to save the key and values to (and move checkheaders elsewhere)
		// ORR give a bool and put a conditional in there but less pretty
		//OR parse manually for members since
		// Content-Disposition: form-data; name="file"; filename="example.txt"
		// AND Content-Type: text/plain are the key ones, could even throw the other ones out
		// if i use table might copy/move the 2 important ones to seperate strings anyways

		nextboundary = _reqBody.find(delimiter, i);
		if (i == std::string::npos)
			throw	ClientErrorExcept(400, "400 no closing boundary in multiform");
		//object.body = _reqBody.substr(boundary + delimiter.size(), nextboundary - boundary)

	}

}