/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/30 15:15:38 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
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

void   location::parseRule(const std::string &line)
{
       auto key = line.begin();
       while (key != line.end() and (*key == ' ' or *key == '\t'))
               key++;
       if (key == line.end() or *key == '#' or *key == '}')
               return;
       auto begin = key;
       while (key != line.end() and *key != ' ' and *key != '\t')
               key++;
               key++;
       std::string tmp_key(begin, key);
       auto value  = key;
       while(value != line.end() and (*value == ' ' or+ *value == '\t'))
               value++;
       auto begin_value = value;
       while(begin_value != line.end() and *begin_value != ';')
               begin_value++;
       if (begin_value == line.end() || *begin_value != ';')
       return;
       std::string tmp_value(value, begin_value);      
       _rulemap[tmp_key] = tmp_value;
}