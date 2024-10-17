/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:39:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/17 20:15:02 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(int clientFD):
	 _clientFD(clientFD), _doneReading(false), reading_mode(NOT_STARTED), body_bytes_read(0), \
	  _rnrnFound(false), _headerAreParsed(false), _method_type(NOT_PARSED_YET), \
	  _keepOpen(true), _statusCode("0 Not started yet")
{
	_rawRequestData.reserve(50); //mb 100
}

Request::Request(const Request &rhs)
{
	std::cout << GREEN << "Http_request: Copy constructor called" << RESET << std::endl;
	*this = rhs;
}

Request &
Request::operator=(const Request &rhs)
{
	std::cout << GREEN << "Request: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_clientFD = rhs._clientFD;
		_URI = rhs._URI;
		_method_type = rhs._method_type;
		_http_version = rhs._http_version;
		_headers = rhs._headers;
		_rawRequestData = rhs._rawRequestData;
		_unsortedHeaders = rhs._unsortedHeaders;
		_rnrnFound = rhs._rnrnFound;
		body_bytes_read = rhs.body_bytes_read;
		reading_mode = rhs.reading_mode;
	}

	return (*this);
}

Request::~Request(void)
{
	std::cout << RED << "http_request: Destructor called" << RESET << std::endl;
}
