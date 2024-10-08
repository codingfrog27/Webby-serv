/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Connection.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: asimone <asimone@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/02 16:26:31 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/03 17:46:55 by asimone		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Connection.hpp"


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Connection::Connection(Config *config, int clientFD): _clientFD(clientFD)
{
	config = config;
	_request = nullptr;

	std::cout << GREEN << "Connection: Default constructor called" << RESET << std::endl;
}


// Connection::Connection(const Connection &rhs)
// {
// 	std::cout << GREEN << "Connection: Copy constructor called" << RESET << std::endl;

// 	*this = rhs;
// }

// Connection &
// Connection::operator=(const Connection &rhs)
// {
// 	std::cout << GREEN << "Connection: Assignment operator called" << RESET << std::endl;

// 	if (this != &rhs)
// 	{
// 		// Perform deep copy
// 	}

// 	return (*this);
// }

Connection::~Connection(void)
{
	std::cout << RED << "Connection: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

// int Connection::Create_Connection(void)
// {
	
// }
