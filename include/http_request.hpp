/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 18:45:48 by mde-cloe         ###   ########.fr       */
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
	FINISHED,
	FINISHED_NO_BODY,
};

class Http_request
{
	private:
		Http_method				_method_type; //should mb be public else i can make getters
		reading_status			reading_mode;
		bool					_body_found;
		int						body_bytes_read;
		const int				_max_body_size = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER (mb rename to _max_size)
		std::string				unsorted_headers;
		std::string				_boundary;
		std::vector<char>		raw_request_data;
		


		int					read_from_socket(int client_fd);
		void				parse_headers(std::string str);
		Http_method			which_method_type(std::string &str);
		void				look_for_body(int bytes_read);
		void				main_reader(int client_fd);
		void				parse_req_line(std::string req_line);

	public:
		std::unordered_map<std::string, std::string> _headers;

		// Constructors and Destructors
		Http_request(void) = delete;
		Http_request(int client_fd);
		Http_request(const Http_request &rhs);
		Http_request &operator=(const Http_request &rhs);
		~Http_request(void);

	float					_http_version;
		bool				_is_cgi;
		std::string			request_line;
	size_t					_content_length;
		std::string filepath; //rename to URI ?
} ;

//     std::string document_root = "/var/www/html"; // Example document root
