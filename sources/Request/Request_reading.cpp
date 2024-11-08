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

//body is written like var=value&var2=value2 etc

void	Request::readRequest()
{
	try
	{
		if (_statusCode == "0 Not started yet")
			_statusCode = "102 Processing";
		if (!_rnrnFound)
		{
			readSocket(0);
			look_for_body();
			if (_rnrnFound && !_headerAreParsed)
				parse_headers(_unsortedHeaders);
		}
		else if (!_doneReading && _dataIsChunked)
			readBody();
		else if (!_doneReading)
			readBody();
		// timeout check here?
	}
	catch(const std::ios_base::failure &e) //custom exception class
	{
		std::cerr << e.what() << std::endl;
	}
	catch(ClientErrorExcept &e)
	{
		std::cerr << e.what() << std::endl;
		// if we want custom payload too l"\nbytes read sofar = "<< body_bytes_read
		//<< "\nwhile allowed amount = "<< _max_body_size << std::endl;
	}
	catch(std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int	Request::readSocket(int size)
{
	if (!size)
		size = BUFFER_SIZE;
	char buffer[size];
	int	bytes_read = recv(_clientFD, buffer, size, MSG_DONTWAIT); //more flags
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			throw(ConnectionClosedExcep(_clientFD));
		else
			throw (std::ios_base::failure("reading fail when reading from client socket"));
	}
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	return (bytes_read);
}

void	Request::look_for_body()
{
	static const std::vector< unsigned char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<unsigned char>::iterator it = std::search(_rawRequestData.begin(),\
	_rawRequestData.end(),body_delim.begin(), body_delim.end());
	if (it == _rawRequestData.end())
		reading_mode = READING_HEADERS;
	else
	{
		_rnrnFound = true;
		_unsortedHeaders = std::string(_rawRequestData.begin(), it); //cut of the rnrn?
		_rawRequestData.erase(_rawRequestData.begin(), it);
		body_bytes_read = _rawRequestData.size();
	}
}


int	Request::convertChunkSize(const std::string &hexStr, size_t rnPos)
{
	size_t		chunkSize;
	char		*hexEnd;
	chunkSize = std::strtol(hexStr.data(), &hexEnd ,16); //alt: use parse_hex
	if (*hexEnd != '\r')
			throw ClientErrorExcept(400, "bad hex number in chunk");
	if (chunkSize + body_bytes_read > _max_body_size)
		throw ClientErrorExcept(413, "payload too large");

}

void	Request::readBody()
{
	if (body_bytes_read + BUFFER_SIZE > _contentLen)
	{	
		body_bytes_read += readSocket(_contentLen - body_bytes_read);
		_doneReading = true;
		reading_mode = FINISHED;
	}
	else
		body_bytes_read += readSocket(0);
	if (body_bytes_read > _max_body_size)
			throw (std::length_error("413 Payload too large"));
}