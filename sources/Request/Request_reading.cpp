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
			readSocket(0); //what should standard size be, could even change it depending on nbr of connects?
			look_for_body();
			if (_rnrnFound && !_headerAreParsed)
				parse_headers(_unsortedHeaders);
		}
		else if (!_doneReading)
			readBody();
		// timeout check here?
	}
	catch(const std::ios_base::failure &e) //custom exception class
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

int	Request::readSocket(int size)
{
	if (!size)
		size = BUFFER_SIZE;
	char buffer[size];
	int	bytes_read = recv(_clientFD, buffer, size, MSG_DONTWAIT); //more flags
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
		{
			std::cout << "Client closed the connection." << std::endl;
			//handle by setting error occured bool something
		}
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
	_rawRequestData.end(),body_delim.begin(), body_delim.end()); //is search allowed?
	
	if (reading_mode != FINISHED && it == _rawRequestData.end())
		reading_mode = READING_HEADERS;
	else
	{
		_rnrnFound = true;
		_unsortedHeaders = std::string(_rawRequestData.begin(), it); //cut of the rnrn?
		_rawRequestData.erase(_rawRequestData.begin(), it);
		body_bytes_read = _rawRequestData.size();
		// if (reading_mode != FINISHED)
		// 	reading_mode = READING_BODY;
	}
}

void	Request::readBody()
{
	if (reading_mode == READING_BODY_CHUNKED)
	{
		std::cout << "to be added" << std::endl;
		exit(1);
	}
	else
	{
		body_bytes_read += readSocket(_contentLen); //divide in chunks if too big and check if not
		_doneReading = true;
	}



	if (body_bytes_read > _max_body_size)
			throw (std::length_error("413 Payload too large"));
}

// test
