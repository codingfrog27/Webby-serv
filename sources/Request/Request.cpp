/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/17 19:39:08 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/03/18 17:09:50 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fcntl.h>

Request::Request(Config *config, int clientFD): _config(config), \
	reading_mode(NOT_STARTED), body_bytes_read(0), _headerEndFound(false), \
	_headerAreParsed(false), _hasBody(false), _root(config->_rootDir), \
	 _clientFD(clientFD), _method_type(Http_method::NOT_PARSED_YET), _keepOpen(true),\
	  _doneReading(false), _statusStr("0 Not started yet"), _dirListing(false)
{
	fcntl(_clientFD, F_SETFL, O_NONBLOCK);
	_rawRequestData.reserve(100);
	_timeoutTime = setTimeout(30); //normal secs
	_startTime = getStartTime();
}

Request::Request(const Request &rhs)
{
	std::cout << GREEN << "Http_request: Copy constructor called" << RESET << std::endl;
	*this = rhs;
}

Request &
Request::operator=(const Request &rhs)
{
	// std::cout << GREEN << "Request: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_config = rhs._config;
		_reqBody = rhs._reqBody;
		_unsortedHeaders = rhs._unsortedHeaders;
		reading_mode = rhs.reading_mode;
		_boundary = rhs._boundary;
		body_bytes_read = rhs.body_bytes_read;
		_headerEndFound = rhs._headerEndFound;
		_dataIsChunked = rhs._dataIsChunked;
		_headerAreParsed = rhs._headerAreParsed;
		_hasBody = rhs._hasBody;
		_contentLen = rhs._contentLen;
		_timeoutTime = rhs._timeoutTime;
		_startTime = rhs._startTime;
		_clientFD = rhs._clientFD;
		_root = rhs._root;
		// _max_body_size = rhs._max_body_size; //IMPLEMENT ME PLS
		_rawRequestData = rhs._rawRequestData;
		_method_type = rhs._method_type;
		_headers = rhs._headers;
		_wwwFormEncodedPairs = rhs._wwwFormEncodedPairs;
		_http_version = rhs._http_version;
		request_line = rhs.request_line;
		_URI = rhs._URI;
		_filePath = rhs._filePath;
		_keepOpen = rhs._keepOpen;
		_doneReading = rhs._doneReading;
		_statusStr = rhs._statusStr;
		_statusCode = rhs._statusCode;
		// _CGIRequired = rhs._CGIRequired;
		_dirListing = rhs._dirListing;
	}

	return (*this);
}

Request::~Request(void)
{
	// std::cout << RED << "http_request: Destructor called" << RESET << std::endl;
}



Config*	Request::getConfig()
{
        return (this->_config);
}