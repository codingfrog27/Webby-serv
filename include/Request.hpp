/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::		*/
/*   Request.hpp									:+:	  :+:	:+:			*/
/*													+:+ +:+		 +:+		 */
/*	By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+		+#+			*/
/*												+#+#+#+#+#+	+#+				*/
/*	Created: 2024/09/06 17:18:08 by mde-cloe		  #+#	#+#				 */
/*	Updated: 2024/10/03 18:18:36 by mde-cloe		 ###	########.fr		*/
/*																			*/
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <unordered_map>
#include <sys/socket.h>
#include "libft.h"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BUFFER_SIZE 20
#define PLACEHOLDER_MAX_SIZE 10240



enum reading_status
{
	NOT_STARTED,
	READING_HEADERS,
	READING_BODY,
	READING_BODY_CHUNKED,
	FINISHED
};

enum Http_method
{
	INVALID = -1,
	GET,
	POST,
	DELETE,
	NOT_PARSED_YET,
};

class Request 
{
	// things to pass/inheret from _config
	// max body size
	// allowed methods
	private:
	// Connection				*_connection;
	std::string				_unsortedHeaders;
	reading_status			reading_mode;
	std::string				_boundary;
	int						body_bytes_read;
	bool					_rnrnFound;
	bool					_dataIsChunked;
	bool					_headerAreParsed;
	size_t					_contentLen; //need to put in init list
	const int				_max_body_size = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER


	int				readSocket(int size);
	void			parse_headers(std::string str);
	Http_method		which_method_type(std::string str);
	void			look_for_body();
	size_t			parse_req_line(std::string req_line);
	void			dechunkBody();
	void			parseBody();
	float			http_version(std::string version);
	void			checkHeaders();
	void			readBody();

	public:
	int							_clientFD;
	std::vector<unsigned char>	_rawRequestData;
	Http_method					_method_type;
	std::unordered_map\
	<std::string, std::string>	_headers;
	float						_http_version;
	std::string					request_line;
	std::string 				_URI; 
	bool						_keepOpen;
	bool						_error_occured;
	bool						_doneReading;
	std::string					_statusCode;
	// Constructors and Destructors
					Request(void) = delete;
					Request(int _clientFD);
					Request(const Request &rhs);
	Request 		&operator=(const Request &rhs);
					~Request(void);
	//public methods
	void			readRequest();
	std::string		getHeaderValue(std::string key);
	bool			headerExists(std::string key);
	std::string		getStatusCode();
	void			printHeaders();
};

