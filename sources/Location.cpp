/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/04 16:00:03 by asimone          ###   ########.fr       */
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
//								GETTER & SETTER								  //
// ************************************************************************** //

std::string	location::getRoot()
{
	return(this->_root);
}

void	location::setRoot(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_root = _rulemap.at(key);
}

std::string	location::getAlias()
{
	return(this->_alias);
}

void	location::setAlias(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_alias = _rulemap.at(key);
}

std::string	location::getAllowMethods()
{
	return(this->_allow_methods);
}

void	location::setAllowMethods(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_allow_methods = _rulemap.at(key);
}

std::string	location::getAutoindex()
{
	return(this->_autoindex);
}

void	location::setAutoindex(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_autoindex = _rulemap.at(key);
}

std::string	location::getCgiExtension()
{
	return(this->_cgi_extension);
}

void	location::setCgiExtension(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_cgi_extension = _rulemap.at(key);
}

std::string	location::getCgiPath()
{
	return(this-> _cgi_path);
}

void	location::setCgiPath(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this-> _cgi_path = _rulemap.at(key);
}

std::string	location::getReturn()
{
	return(this->_return);
}

void	location::setReturn(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_return = _rulemap.at(key);
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

std::string location::toString() const {
    std::ostringstream oss;
    return oss.str();
}

void	location::initializeLocation()
{
	setRoot("root");
	setAlias("alias");
	setAllowMethods("allow_methods");
	setAutoindex("autoindex");
	setCgiExtension("cgi_extension");
	setCgiPath("cgi_path");
	setReturn("return");
}

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
       _rulemap.emplace(tmp_key, tmp_value);
	   initializeLocation();
}