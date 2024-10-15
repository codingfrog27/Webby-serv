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

#include "HttpRequest.hpp"

// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //


Request::Request(int client_fd):
	 _clientFD(client_fd), reading_mode(NOT_STARTED), body_bytes_read(0), \
	  _bodyFound(false), _headerAreParsed(false), _method_type(NOT_PARSED_YET), \
	  _keepOpen(true), _statusCode("0 Not started yet")
{
	std::cout << GREEN << "Http_request parsing started" << RESET << std::endl;
	// main_reader();
	std::cout << "" << std::endl;
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
		_URI = rhs._URI;
		_method_type = rhs._method_type;
		_http_version = rhs._http_version;
		_headers = rhs._headers;
		_rawRequestData = rhs._rawRequestData;
		_unsortedHeaders = rhs._unsortedHeaders;
		_bodyFound = rhs._bodyFound;
		body_bytes_read = rhs.body_bytes_read;
		reading_mode = rhs.reading_mode;
	}

	return (*this);
}

Request::~Request(void)
{
	std::cout << RED << "http_request: Destructor called" << RESET << std::endl;
}

//body is written like var=value&var2=value2 etc
// linked list for headers?


// ************************************************************************** //
//						  main reading methods							  //
// ************************************************************************** //



void	Request::readRequest()
{
	try
	{
		read_from_socket();
		if (!_bodyFound)
			look_for_body();
		if (body_bytes_read > _max_body_size)
			throw (std::length_error("413 Payload too large"));
		if (reading_mode != READING_HEADERS && !_headerAreParsed)
			parse_headers(_unsortedHeaders);
		if (reading_mode == FINISHED && _bodyFound)
			parseBody();
		// timeout check here?
		if (_statusCode == "0 Not started yet") // or others?
			_statusCode = "102 Processing";
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

void	Request::read_from_socket()
{
	static char buffer[BUFFER_SIZE] = {0};
	int	bytes_read = read(_clientFD, buffer, BUFFER_SIZE - 1);
	
	if (bytes_read < 0)
		throw (std::ios_base::failure("reading fail when reading from client socket"));
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	if (reading_mode == READING_BODY)
		body_bytes_read += bytes_read;
	if (bytes_read < BUFFER_SIZE - 1)
		reading_mode = FINISHED;
}

void	Request::look_for_body()
{
	static const std::vector<char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<char>::iterator it = std::search(_rawRequestData.begin(),\
	_rawRequestData.end(),body_delim.begin(), body_delim.end()); //is search allowed?
	
	if (reading_mode != FINISHED && it == _rawRequestData.end())
		reading_mode = READING_HEADERS;
	else
	{
		_bodyFound = true;
		_unsortedHeaders = std::string(_rawRequestData.begin(), it); //cut of the rnrn?
		_rawRequestData.erase(_rawRequestData.begin(), it);
		body_bytes_read = _rawRequestData.size();
		if (reading_mode != FINISHED)
			reading_mode = READING_BODY;
	}
}

// test
