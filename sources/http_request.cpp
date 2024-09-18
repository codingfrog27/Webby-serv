/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:22:52 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 15:36:23 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_request.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Http_request::Http_request(int client_fd): _is_cgi(false), _has_body(true), _body_found(false), \
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
	std::cout << this->raw_request_data; //print request for testing

	
	

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
	if (!_body_found)
		look_for_body(bytes_read);
	if (body_bytes_read > _max_body_size)
		throw (std::length_error("Request size exceeds the allowed limit"));
	if (reading_mode != READING_HEADERS) //&& headers_not_parsed_yet bool or check one of the values it sets)
		parse_headers(unsorted_headers);
		
}

int	Http_request::read_from_socket(int client_fd)
{
	static char buffer[BUFFER_SIZE] = {0};
	int	bytes_read;
	
	bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
	if (bytes_read < 0)
		throw (std::ios_base::failure("reading fail when reading from client socket"));
	raw_request_data.insert(raw_request_data.end(), buffer, buffer + bytes_read);
	if (reading_mode == READING_BODY)
		body_bytes_read += bytes_read;
	return (bytes_read);
}

void	Http_request::look_for_body(int bytes_read)
{
	static const std::vector<char> body_delim = {'\r', '\n', '\r', '\n'};
	std::vector<char>::iterator it = std::search(raw_request_data.begin(), raw_request_data.end(),\
	body_delim.begin(), body_delim.end()); //is search allowed?
	
	if (it != raw_request_data.end())
	{
		_body_found = true;
		unsorted_headers = std::string(raw_request_data.begin(), it); //cut of the rnrn?
		raw_request_data.erase(raw_request_data.begin(), it + body_delim.size());
		body_bytes_read = raw_request_data.size();
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

void	Http_request::parse_headers(std::string str)
{
	std::unordered_map<std::string, std::string> headers;














	std::string method, url, http_version;

	int	newline = unsorted_headers.find('\n');
	if (newline == std::string::npos)
		throw (std::length_error("Request line is too long!")); //set up right now so that it erros if req line is longer than 1024
	std::istringstream request_stream(unsorted_headers.substr(0, newline));
	request_stream >> method >> url >> http_version;
	// First line splitting from chatgpt, not sure if safe yet



	_method_type = which_method_type(method);
	// filepath = urlToFilePath(url);
	// _http_version =
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

float http_version(std::string &version)
{
	if (version.compare(0, 5, "HTTP/") != 0 || version.size() > 8)
		throw std::invalid_argument("Unsupported HTTP version: " + version);
	return (std::stof(version.substr(5)));
}

// std::string Http_request::urlToFilePath(const std::string& url) {
//     // Ensure the URL starts with a '/'
//     if (url.empty() || url[0] != '/') {
//         throw std::invalid_argument("Invalid URL");
//     }

//     // Sanitize the URL to prevent directory traversal attacks
//     std::string sanitized_url;
//     for (size_t i = 0; i < url.size(); ++i) {
//         if (url[i] == '.' && i + 1 < url.size() && url[i + 1] == '.') {
//             // Skip ".." to prevent directory traversal
//             i++;
//         } else {
//             sanitized_url += url[i];
//         }
//     }

//     return file_path;
// }
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



// KNOWLEDGEEEEEE

// CHAR VECTOR
// dont worry about how u parse headers too much? map/save importo
// look for delimiter within body called somehting =
// 
// 