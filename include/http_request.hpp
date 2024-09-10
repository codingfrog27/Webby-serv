/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:18:08 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/10 20:07:55 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BUFFER_SIZE 1024

enum Http_method
{
	GET,
	POST,
	DELETE,
};

class Http_request
{
	private:
		Http_method	_method_type; //should mb be public else i can make getters
		bool		_is_cgi;

		void	socket_to_string(int client_fd);

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
} ;

//     std::string document_root = "/var/www/html"; // Example document root
