/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 18:18:36 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <unordered_map>
#include "libft.h"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BUFFER_SIZE 1024
#define PLACEHOLDER_MAX_SIZE 10240

enum Http_method //update parse first read if this changes
{
	INVALID = -1,
	GET,
	POST,
	DELETE,
	NOT_PARSED_YET,
};

enum reading_status
{
	NOT_STARTED,
	READING_HEADERS,
	READING_BODY,
	FINISHED
};

class HttpRequest 
{
	// things to pass/inheret from config
	// max body size
	// allowed methods
	private:
	int						_clientFD;
	std::string				_unsortedHeaders;
	reading_status			reading_mode;
	std::string				_boundary;
	int						body_bytes_read;
	bool					_bodyFound;
	bool					_dataIsChunked;
	bool					_headerAreParsed;
	const int				_max_body_size = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER


	void				read_from_socket(int client_fd);
	void				parse_headers(std::string str);
	Http_method			which_method_type(std::string str);
	void				look_for_body();
	size_t				parse_req_line(std::string req_line);
	void				dechunkBody();
	void				parseBody();
	float				http_version(std::string version);
	void				checkHeaders();

	public:
	// Constructors and Destructors
	void				main_reader(int client_fd);
	HttpRequest(void);
	HttpRequest(int client_fd);
	HttpRequest(const HttpRequest &rhs);
	HttpRequest &operator=(const HttpRequest &rhs);
	~HttpRequest(void);
	std::string	getHeaderValue(std::string key);
	bool	headerExists(std::string key);
	std::string		getStatusCode();


	std::vector<char>								_rawRequestData;
	Http_method										_method_type; //should mb be public else i can make getters
	std::unordered_map<std::string, std::string> 	_headers;
	float											_http_version;
	std::string										request_line;
	std::string 									_URI; 
	bool											_keepOpen;
	bool											_error_occured;
	std::string										_statusCode;
};
    // std::string document_root = "/var/www/html"; // Example document root
