/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionClosedExcep.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:20:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/01/06 13:49:37 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionClosedExcep.hpp"


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

ConnectionClosedExcep::ConnectionClosedExcep(int clientFD) : _clientFD(clientFD)
{
	// std::cout << GREEN << "ConnectionClosedExcep: Default constructor called" << RESET << std::endl;
}

ConnectionClosedExcep::ConnectionClosedExcep(const ConnectionClosedExcep &rhs) : _clientFD(rhs._clientFD)
{
	// std::cout << GREEN << "ConnectionClosedExcep: Copy constructor called" << RESET << std::endl;
}

ConnectionClosedExcep::~ConnectionClosedExcep(void)
{
	// std::cout << RED << "ConnectionClosedExcep: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

//is based on recv reading 0 even with POLLIN
const char		*ConnectionClosedExcep::what() const noexcept
{
	return ("Client has closed the connection");
}