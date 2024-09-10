/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:22:52 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/10 17:28:05 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_request.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Http_request::Http_request(int client_fd)
{
	std::cout << GREEN << "Http_request parsing started" << RESET << std::endl;
	try
	{
		socket_to_string(client_fd);
	}
	catch(const std::ios_base::failure &e)
	{
		std::cerr << e.what() << std::endl;
		//handle reading error?
	}
	std::cout << this->unsorted_string;
	

}


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

void	Http_request::socket_to_string(int client_fd)
{
	char buffer[BUFFER_SIZE] = {0};
	int	bytes_read;
	do
	{
		std::cout << "here?\n";
		bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
		if (bytes_read < BUFFER_SIZE - 1)
		{
			if (bytes_read < 0)
				throw (std::ios_base::failure("reading fail when reading from client socket"));
		
		}
		buffer[bytes_read] = '\0';
		this->unsorted_string += buffer;
	} while (bytes_read == BUFFER_SIZE - 1);
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //


