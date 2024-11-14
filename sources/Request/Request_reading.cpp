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
		readSocket(0);
		if (!_rnrnFound)
		{
			if (look_for_body()) //make 1 if
				parse_headers(_unsortedHeaders);
		}
		if (_hasBody && bodyIsRead()) {
			std::cout << "PARSING BODY" << std::endl;
			parseBody();
		} //make hasBody
		// timeout check here?
	}
	catch(ClientErrorExcept &e)
	{
		std::cerr << e.what() << std::endl;
		// if we want custom payload too l"\nbytes read sofar = "<< body_bytes_read
		//<< "\nwhile allowed amount = "<< _max_body_size << std::endl;
	}
	catch(const std::ios_base::failure &e) //custom exception class
	{
		std::cerr << e.what() << std::endl;
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
	// int	bytes_read = recv(_clientFD, buffer, size, MSG_DONTWAIT); //more flags
	int	bytes_read = read(_clientFD, buffer, size); //more flags
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			throw(ConnectionClosedExcep(_clientFD));
		else
			throw (std::ios_base::failure("reading fail when reading from client socket"));
	}
	_rawRequestData.insert(_rawRequestData.end(), buffer, buffer + bytes_read);
	if (_rnrnFound)
		body_bytes_read += bytes_read;
	return (bytes_read);
}

bool	Request::look_for_body()
{
	static const std::vector< unsigned char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<unsigned char>::iterator it = std::search(_rawRequestData.begin(),\
	_rawRequestData.end(),body_delim.begin(), body_delim.end());
	if (it == _rawRequestData.end()) {
		reading_mode = READING_HEADERS;
		return (false);
	}
	_rnrnFound = true;
	_unsortedHeaders = std::string(_rawRequestData.begin(), it + 2);
	_rawRequestData.erase(_rawRequestData.begin(), it + 2); //cut out the first CRLF
	body_bytes_read = _rawRequestData.size();
	return (true);
}

bool	Request::bodyIsRead()
{
	if (body_bytes_read > _max_body_size)
			throw (ClientErrorExcept(413, "413 Payload too large"));
	if (_dataIsChunked && dechunkBody() == true)
		return(true);
	else if (body_bytes_read >= _contentLen)
	{	
		_doneReading = true;
		reading_mode = FINISHED;
		_reqBody = std::string(_rawRequestData.begin() + 2,(_rawRequestData.begin() + _contentLen));
		return (true);
	}
	return (_doneReading);
}