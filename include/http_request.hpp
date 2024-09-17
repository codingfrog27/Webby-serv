/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/17 19:04:29 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
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
	FINISHED,
	FINISHED_NO_BODY,
};

class Http_request
{
	private:
		Http_method				_method_type; //should mb be public else i can make getters
		reading_status			reading_mode;
		bool					_is_cgi;
		int						body_bytes_read;
		int						_bytes_read;
		const int				_max_body_size = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER (mb rename to _max_size)
		float					_http_version;
		size_t					_content_length;
		std::string				_boundary;
		const std::vector<char> body_start = {'\r', '\n', '\r', '\n'};
		// bool					_has_body;


		int					read_from_socket(int client_fd);
		void				parse_headers(std::string str);
		Http_method			which_method_type(std::string &str);
		reading_status		look_for_body(void);
		void				main_reader(int client_fd);

	public:
		// Constructors and Destructors
		Http_request(void) = delete;
		Http_request(int client_fd);
		Http_request(const Http_request &rhs);
		Http_request &operator=(const Http_request &rhs);
		~Http_request(void);

		// Public Methods
		std::vector<char>	raw_request_data;
		std::string			unsorted_headers;
		std::string			request_line;
		std::string filepath; //rename to URI ?
} ;

//     std::string document_root = "/var/www/html"; // Example document root
