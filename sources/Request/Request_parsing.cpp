/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:31:50 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/12 20:44:45 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   HttpRequest_parsing.cpp							:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/24 14:11:35 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/04 18:04:40 by mde-cloe		 ###   ########.fr	   */
/*																			*/
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
	_URI = req_line.substr(method_end + 1, uri_end - method_end - 1);
	_http_version = http_version(&req_line[uri_end + 1]);
	return (line_end + 2);
}

// not trimming any trailing whitespace rn cause RFC 7230 states its not allowed
// some irl servers do allow it though.. so might add later?
void	Request::parse_headers(std::string header_str)
{
	size_t						start, colon_pos, line_end;
	std::string					key, value;

	start = parse_req_line(header_str);
	line_end = header_str.find("\r\n", start);
	while (line_end != std::string::npos)
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
	if (!headerExists("Host")) //split?
		throw(std::invalid_argument("400 bad request: Host missing"));
	if (getHeaderValue("Connection") == "close")
		_keepOpen = false;
	else if (getHeaderValue("Connection") == "keep-alive")
		_keepOpen = true;
	// else
	// 	throw(std::invalid_argument("400 bad request: Connection missing"));
	if (_method_type == POST || _method_type == DELETE)
	{
		if (!headerExists("Content-Type")) //or if type not supported
			throw (std::invalid_argument("415 Bad request. Unsupported Media Type"));
		if (headerExists("content-length"))
		{
			_contentLen = std::stoul(getHeaderValue("content-length")); //handle exceptions?
			if (_contentLen == 0)
				throw (std::invalid_argument("411 length required"));
			if (_contentLen > _max_body_size)
				throw (std::length_error("413 Payload too large"));
			reading_mode = READING_BODY;
		}
		else if (getHeaderValue("Transfer-Encoding") != "chunked")
			throw (std::invalid_argument("411 length required")); //could seperate to be more precise
		else
			reading_mode = READING_BODY_CHUNKED;
			
		//expect 100 continue?/
	}
	else //bodys in GET, get ignored :)
	{
		_doneReading = true;
		// _statusCode = "200 OK";
		_statusCode = "";
	}
	//mb implement timeout mechanism since malicious requests could send body without these headers
}

//not sure but Authorization if thats in scope for protected resources
//accept
//not mandated but could return 406 Not Acceptable
// look into expext, mb range,
//  if-modified-since/if-none-match 
// ^ (could contain misuse but else return 304) 



//almost done! just add logic so we can erase once after the loop by keeping track of our last found rn
//wait doesnt completelt ofjeoifwajdoiwa time to go homee
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
		if (chunkSize == 0)
			_doneReading = true;
		else
			_reqBody += bodyStr.substr(rnPos + hexStrSize, chunkSize);
		bytesParsed += rnPos + hexStrSize + chunkSize;
	}
	_rawRequestData.erase(_rawRequestData.begin(), (_rawRequestData.begin() + bytesParsed)); 
	//do i have to remove 2 more if we're at the end? if so can make chunksize 2 :)
	return (_doneReading);
}


void	Request::parseBody()
{
	const std::string	suffix = "--";
		std::string		content_type = getHeaderValue("Content-Type"); //make ref?
		std::string		delimiter;
		//assuming its there cause of header check
	
		
	if (getHeaderValue("transfer encoding") == "chunked")
		_dataIsChunked = true;
	if(content_type.compare("multipart/form-data; boundary=") == 0)
	{
		if (content_type.size() < 31) //meaning multiform without boundery!
			throw(ClientErrorExcept(400, "400, Bad Request, empty boundary parameter"));
		delimiter = content_type.substr(31);
	}
}
	// If any transfer coding other than chunked is applied to a request's
	//  content, the sender MUST apply chunked as the final transfer coding 
	//  to ensure that the message is properly framed. 
	// If any transfer coding other
	//  than chunked is applied to a response's content, the sender MUST either
	//   apply chunked as the final transfer coding or terminate the message by closing the connection.


//content encoding bestaat ook nog
//check for content length == body bytes read
	
//multipart/form data
