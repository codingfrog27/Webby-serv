/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:22:52 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/23 18:35:30 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_request.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Http_request::Http_request(int client_fd): _is_cgi(false), _has_body(true), _bodyFound(false), \
		_method_type(NOT_PARSED_YET), body_bytes_read(0), reading_mode(NOT_STARTED) //add max bytes read from
{
	std::cout << GREEN << "Http_request parsing started" << RESET << std::endl;
	try
	{
		read_from_socket(client_fd);
		parse_headers();
	}
	catch(const std::ios_base::failure &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(std::length_error &e)
	{
		std::cerr << e.what() << "\nbytes read sofar = "<< body_bytes_read\
		<< "\nwhile allowed amount = "<< _max_body_size << std::endl;
		// also catches too long req error, split up?
		//and stof?
	}
	catch(std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
	//handle  errors
	// std::cout << this->_rawRequestData; //print request for testing

	
	

}

//body is written like var=value&var2=value2 etc
// linked list for headers?

Http_request::Http_request(const Http_request &rhs)
{
	std::cout << GREEN << "Http_request: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Http_request &
Http_request::operator=(const Http_request &rhs)
{
	std::cout << GREEN << "Http_request: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		// Perform deep copy
	}

	return (*this);
}

Http_request::~Http_request(void)
{
	std::cout << RED << "http_request: Destructor called" << RESET << std::endl;
}

void	Http_request::main_reader(int client_fd)
{
	int bytes_read = read_from_socket(client_fd);
	if (!_bodyFound)
		look_for_body(bytes_read);
	if (body_bytes_read > _max_body_size)
		throw (std::length_error("Request size exceeds the allowed limit"));
	if (reading_mode != READING_HEADERS) //&& headers_not_parsed_yet bool or check one of the values it sets)
		parse_headers(_unsortedHeaders);
		
}

int	Http_request::read_from_socket(int client_fd)
{
	static char buffer[BUFFER_SIZE] = {0};
	int	bytes_read;
	
	bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
	if (bytes_read < 0)
		throw (std::ios_base::failure("reading fail when reading from client socket"));
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	if (reading_mode == READING_BODY)
		body_bytes_read += bytes_read;
	return (bytes_read);
}

void	Http_request::look_for_body(int bytes_read)
{
	static const std::vector<char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<char>::iterator it;
	
	it = std::search(_rawRequestData.begin(), _rawRequestData.end(),\
	body_delim.begin(), body_delim.end()); //is search allowed?
	
	if (it != _rawRequestData.end())
	{
		_bodyFound = true;
		_unsortedHeaders = std::string(_rawRequestData.begin(), it + 2); //cut of the rnrn?
		_rawRequestData.erase(_rawRequestData.begin(), it + body_delim.size());
		body_bytes_read = _rawRequestData.size();
		if (bytes_read < BUFFER_SIZE - 1)
			reading_mode = FINISHED;
		else
			reading_mode = READING_BODY;
	}
	else if (bytes_read < BUFFER_SIZE - 1)
		reading_mode = FINISHED_NO_BODY; //could also just be finished and look at the bool for body found
	else
		reading_mode = READING_HEADERS;
}


// not trimming any trailing whitespace rn cause RFC 7230 states its not allowed
// some irl servers do allow it though.. so might add later?
void	Http_request::parse_headers(std::string header_str)
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

size_t	Http_request::parse_req_line(std::string req_line)
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

Http_method Http_request::which_method_type(std::string str) //will be updated after conig parsing
{
	const char *Methods[] = {"GET ", "POST ", "DELETE "}; 

	for (size_t i = 0; i < 3; i++)
	{
		if (str == Methods[i])
			return (static_cast<Http_method>(i));
	}
	throw std::invalid_argument("Unsupported HTTP method: " + str);
}


void Http_request::dechunkBody()
{
	
}

float http_version(std::string version)
{
	if (version.compare(0, 5, "HTTP/") != 0 || version.size() > 8)
		throw std::invalid_argument("Unsupported HTTP version: " + version);
	return (std::stof(version.substr(5))); //catch potential exceptions from stof?
}
