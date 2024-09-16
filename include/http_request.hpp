/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/16 20:34:46 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
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
};

class Http_request
{
	private:
		Http_method	_method_type; //should mb be public else i can make getters
		bool		_is_cgi;
		bool		_more_to_read; //change to reading status
		int			_total_bytes_read;
		const int	_max_bytes_to_read = PLACEHOLDER_MAX_SIZE; //PLACEHOLDER (mb rename to _max_size)
		float		_http_version;

		void		socket_to_string(int client_fd);
		void		parse_first_read(void);
		Http_method	which_method_type(std::string &str);

	public:
		// Constructors and Destructors
		Http_request(void) = delete;
		Http_request(int client_fd);
		Http_request(const Http_request &rhs);
		Http_request &operator=(const Http_request &rhs);
		~Http_request(void);

		// Public Methods
		std::string	unsorted_string;
		std::string	request_line;
		std::string filepath; //rename to URI ?
} ;

//     std::string document_root = "/var/www/html"; // Example document root
