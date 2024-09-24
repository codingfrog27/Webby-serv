/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest_parsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:11:35 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/24 15:58:22 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

// not trimming any trailing whitespace rn cause RFC 7230 states its not allowed
// some irl servers do allow it though.. so might add later?
void	HttpRequest::parse_headers(std::string header_str)
{
	size_t						start, colon_pos;
	std::string					key, value;

	start = parse_req_line(header_str);
	for (size_t line_end = header_str.find("\r\n", start); line_end != std::string::npos;)
	{
		colon_pos = header_str.find(':');
		if (colon_pos == std::string::npos)
			throw (std::invalid_argument("colon missing in header"));
		key = header_str.substr(start, colon_pos - start);
		value = header_str.substr(colon_pos + 1, line_end - colon_pos + 1);
		_headers[key] = value;
		start += line_end + 2;
	}

	

	//done? just check for potential required headers
}

size_t	HttpRequest::parse_req_line(std::string req_line)
{
	size_t	line_end, method_end, uri_end;
	
	line_end = req_line.find("\r\n");
	if (line_end == std::string::npos)
		throw (std::invalid_argument("no CRLF found"));
	method_end = req_line.find(' ');
	if (method_end == std::string::npos)
		throw (std::invalid_argument("no space found after method"));
	uri_end = req_line.find(' ', method_end + 1);
	if (uri_end == std::string::npos)
		throw (std::invalid_argument("no space found after uri"));

	_method_type = which_method_type(req_line.substr(0, method_end));
	_URI = req_line.substr(method_end + 1, uri_end - method_end - 1);
	_http_version = http_version(&req_line[uri_end + 1]); //mb a bit overkill
	return (line_end + 2);
}

Http_method HttpRequest::which_method_type(std::string str) //will be updated after conig parsing
{
	const char *Methods[] = {"GET ", "POST ", "DELETE "}; 

	for (size_t i = 0; i < 3; i++)
	{
		if (str == Methods[i])
			return (static_cast<Http_method>(i));
	}
	throw std::invalid_argument("Unsupported HTTP method: " + str);
}


void HttpRequest::dechunkBody()
{
	// https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding
}

float http_version(std::string version)
{
	if (version.compare(0, 5, "HTTP/") != 0 || version.size() > 8)
		throw std::invalid_argument("Unsupported HTTP version: " + version);
	return (std::stof(version.substr(5))); //catch potential exceptions from stof?
}

void	HttpRequest::parseBody()
{
	if (_method_type == GET)
		throw (std::invalid_argument("GET requests should not have a body"));
	if (_headers["Transfer-Encoding"] == "chunked")
		dechunkBody();
	
	
}

void	HttpRequest::checkHeaders()
{
	//required headers
	// host //400 Bad Request
	//content-type (content-length || transfer-encoding) if theres a body
	// 411 Length Required or 415 Unsupported Media if type is missing

	//not sure but Authorization if thats in scope for protected resources
	//accept
	//not mandated but could return 406 Not Acceptable


	//check for content length == body bytes read
}

bool	find()
{
	auto it = _headers.find("Transfer-Encoding");
}it != _headers.end() && it->second == "chunked")


// look into expext, mb range, if-modified-since/if-none-match (could contain misuse but else return 304) header
// connection
// 