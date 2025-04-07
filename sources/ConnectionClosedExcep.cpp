/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConnectionClosedExcep.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/08 19:20:55 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/04/07 14:03:36 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionClosedExcep.hpp"


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

ConnectionClosedExcep::ConnectionClosedExcep(int clientFD) : _clientFD(clientFD)
{
}

ConnectionClosedExcep::ConnectionClosedExcep(const ConnectionClosedExcep &rhs) : _clientFD(rhs._clientFD)
{
}

ConnectionClosedExcep::~ConnectionClosedExcep(void)
{
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

//is based on recv reading 0 even with POLLIN
const char		*ConnectionClosedExcep::what() const noexcept
{
	return ("Client has closed the connection: ");
}