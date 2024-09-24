/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/24 15:27:35 by mde-cloe         ###   ########.fr       */
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
		std::vector<char>		_rawRequestData;
		std::string				_unsortedHeaders;
		reading_status			reading_mode;
		int						body_bytes_read;
		std::string				_boundary;
		bool					_bodyFound;
		bool					_dataIsChunked;
		bool					_headerAreParsed;
		const int				_max_body_size = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER


		void				read_from_socket(int client_fd);
		void				parse_headers(std::string str);
		Http_method			which_method_type(std::string str);
		void				look_for_body();
		void				main_reader(int client_fd);
		size_t				parse_req_line(std::string req_line);
		void				dechunkBody();
		void				parseBody();

	public:
		Http_method				_method_type; //should mb be public else i can make getters
		std::unordered_map<std::string, std::string> _headers;

		// Constructors and Destructors
		HttpRequest(void) = delete;
		HttpRequest(int client_fd);
		HttpRequest(const HttpRequest &rhs);
		HttpRequest &operator=(const HttpRequest &rhs);
		~HttpRequest(void);

	float				_http_version;
	bool				_is_cgi;
	std::string			request_line;
	size_t				_content_length;
	std::string 		_URI; //rename to URI ?
};

//     std::string document_root = "/var/www/html"; // Example document root
