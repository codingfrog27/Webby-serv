/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/24 16:50:56 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Colors.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

location::location(void)
{
	std::cout << GREEN << "location: Default constructor called" << RESET << std::endl;
}

location::location(const location &rhs)
{
	// std::cout << GREEN << "location: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

location &
location::operator=(const location &rhs)
{
	// std::cout << GREEN << "location: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		// Perform deep copy
	}

	return (*this);
}

location::~location(void)
{
	std::cout << RED << "location: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

std::string	location::getRoot(const location& locationBlock)
{
	for(const auto& pair : locationBlock)
	{
		if (pair.first == "root")
			return (pair.second);
	}
	return ("default");
}

void	location::setRoot(const std::string& root)
{
	this->_root = root;
	std::cout << "Root set to: " << _root << std::endl;
}

void	location::printLocationValue(const location& loc) const
{
	for (const auto& pair : loc.locationBlock)
		std::cout << pair.first << ": " << pair.second << std::endl;	
}