/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:22:52 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/16 20:34:13 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_request.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Http_request::Http_request(int client_fd): _is_cgi(false), _more_to_read(false),\
		_method_type(NOT_PARSED_YET), _total_bytes_read(0) //add max bytes read from
{
	std::cout << GREEN << "Http_request parsing started" << RESET << std::endl;
	try
	{
		socket_to_string(client_fd);
		parse_first_read();
	}
	catch(const std::ios_base::failure &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(std::length_error &e)
	{
		std::cerr << e.what() << "\nbytes read sofar = "<< _total_bytes_read\
		<< "\nwhile allowed amount = "<< _max_bytes_to_read << std::endl;
		// also catches too long req error, split up?
		//and stof?
	}
	catch(std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
	//handle  errors
	std::cout << this->unsorted_string; //print request for testing

	
	

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

void	Http_request::parse_first_read(void)
{

	std::string method, url, http_version;

	int	newline = unsorted_string.find('\n');
	if (newline == std::string::npos)
		throw (std::length_error("Request line is too long!")); //set up right now so that it erros if req line is longer than 1024
	std::istringstream request_stream(unsorted_string.substr(0, newline));
	request_stream >> method >> url >> http_version;
	// First line splitting from chatgpt, not sure if safe yet



	_method_type = which_method_type(method);
	filepath = urlToFilePath(url);
	_http_version = 
	
	
	
}

float http_version(std::string &version)
{
	if (version.compare(0, 5, "HTTP/") != 0 || version.size() > 8)
		throw std::invalid_argument("Unsupported HTTP version: " + version);
	return (std::stof(version.substr(5)));
}

Http_method Http_request::which_method_type(std::string &str)
{
	const char *Methods[] = {"GET ", "POST ", "DELETE "}; //add more and make static?

	for (size_t i = 0; i < 3; i++) //hardcoded limit? :s
	{
		if (str == Methods[i])
			return (static_cast<Http_method>(i));
	}
	throw std::invalid_argument("Unsupported HTTP method: " + str);
}

void	Http_request::socket_to_string(int client_fd)
{
	char buffer[BUFFER_SIZE] = {0};
	int	bytes_read;
		bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
		if (bytes_read < BUFFER_SIZE - 1)
		{
			if (bytes_read < 0)
				throw (std::ios_base::failure("reading fail when reading from client socket"));
			_more_to_read = false;
		}
		else
			_more_to_read = true;
		_total_bytes_read += bytes_read;
		if (_total_bytes_read > _max_bytes_to_read)
			throw (std::length_error("Request size exceeds the allowed limit"));
		buffer[bytes_read] = '\0';
		this->unsorted_string += buffer;
	//for the multiple reads I think I need either a read before bool to save bytes read and 
}

std::string Http_request::urlToFilePath(const std::string& url) {
    // Ensure the URL starts with a '/'
    if (url.empty() || url[0] != '/') {
        throw std::invalid_argument("Invalid URL");
    }

    // Sanitize the URL to prevent directory traversal attacks
    std::string sanitized_url;
    for (size_t i = 0; i < url.size(); ++i) {
        if (url[i] == '.' && i + 1 < url.size() && url[i + 1] == '.') {
            // Skip ".." to prevent directory traversal
            i++;
        } else {
            sanitized_url += url[i];
        }
    }

    return file_path;
}
// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //


//1 start with /
//2 skip ..
// 


//todo tuesday


// check if part req line == safe
// 
// same for parse URL
// 
// look into URI and if urlto filepath is needed
// compile

// CONSIDERATIONS
// Methods array could be static, a vector and the for loop condition shouldnt be hardcoded
//set up right now so that it erros if req line is longer than 1024