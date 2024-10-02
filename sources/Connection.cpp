/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:26:31 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/02 17:08:48 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Connection.hpp"

#include "Connection.hpp"

#include <iostream>


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Connection::Connection(Config *config) : _socket(config)
{
	_serverFD = _socket._socketFd;

	std::cout << GREEN << "Connection: Default constructor called" << RESET << std::endl;
}


Connection::Connection(const Connection &rhs)
{
	std::cout << GREEN << "Connection: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Connection &
Connection::operator=(const Connection &rhs)
{
	std::cout << GREEN << "Connection: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		// Perform deep copy
	}

	return (*this);
}

Connection::~Connection(void)
{
	std::cout << RED << "Connection: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

// int Connection::Create_Connection(void)
// {
	
// }
