/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest_reading.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:22:52 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/24 15:43:06 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //


HttpRequest::HttpRequest(int client_fd): _is_cgi(false), _bodyFound(false), \
		_method_type(NOT_PARSED_YET), body_bytes_read(0), reading_mode(NOT_STARTED), \
		_headerAreParsed(false)
{
	std::cout << GREEN << "Http_request parsing started" << RESET << std::endl;
	// main_reader(client_fd);
}

HttpRequest::HttpRequest(const HttpRequest &rhs)
{
	std::cout << GREEN << "Http_request: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

HttpRequest &
HttpRequest::operator=(const HttpRequest &rhs)
{
	std::cout << GREEN << "HttpRequest: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_URI = rhs._URI;
		_method_type = rhs._method_type;
		_http_version = rhs._http_version;
		_headers = rhs._headers;
		_rawRequestData = rhs._rawRequestData;
		_unsortedHeaders = rhs._unsortedHeaders;
		_bodyFound = rhs._bodyFound;
		_is_cgi = rhs._is_cgi;
		body_bytes_read = rhs.body_bytes_read;
		reading_mode = rhs.reading_mode;
	}

	return (*this);
}

HttpRequest::~HttpRequest(void)
{
	std::cout << RED << "http_request: Destructor called" << RESET << std::endl;
}

//body is written like var=value&var2=value2 etc
// linked list for headers?


// ************************************************************************** //
//                          main reading methods                              //
// ************************************************************************** //



void	HttpRequest::main_reader(int client_fd)
{
	try
	{
		read_from_socket(client_fd);
		if (!_bodyFound)
			look_for_body();
		if (body_bytes_read > _max_body_size)
			throw (std::length_error("Request size exceeds the allowed limit"));
		if (reading_mode != READING_HEADERS && !_headerAreParsed)
			parse_headers(_unsortedHeaders);
		if (reading_mode == FINISHED && _bodyFound)
			parseBody();
		//timeout check here?
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
}

void	HttpRequest::read_from_socket(int client_fd)
{
	static char buffer[BUFFER_SIZE] = {0};
	int	bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
	
	if (bytes_read < 0)
		throw (std::ios_base::failure("reading fail when reading from client socket"));
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	if (reading_mode == READING_BODY)
		body_bytes_read += bytes_read;
	if (bytes_read < BUFFER_SIZE - 1)
		reading_mode = FINISHED;
}

void	HttpRequest::look_for_body()
{
	static const std::vector<char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<char>::iterator it = std::search(_rawRequestData.begin(),\
	_rawRequestData.end(),body_delim.begin(), body_delim.end()); //is search allowed?
	
	if (reading_mode != FINISHED && it == _rawRequestData.end())
		reading_mode = READING_HEADERS;
	else
	{
		_bodyFound = true;
		_unsortedHeaders = std::string(_rawRequestData.begin(), it + 2); //cut of the rnrn?
		_rawRequestData.erase(_rawRequestData.begin(), it + body_delim.size());
		body_bytes_read = _rawRequestData.size();
		if (reading_mode != FINISHED)
			reading_mode = READING_BODY;
	}
}













