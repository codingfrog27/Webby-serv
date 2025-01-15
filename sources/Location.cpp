/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */  
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "Colors.hpp"
#include <sstream>
#include "NicePrint.hpp"

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

location::location(void)
{
	std::cout << GREEN << "location: Default constructor called" << RESET << std::endl;
}

location::location(std::ifstream &file, std::string &line)
{
	size_t i = 0;
	while (std::getline(file, line))
	{
		if (line.empty() || line[i] == '#')
			continue;
		if (locationFound(line))
			_nestedLocations.emplace_back(file, line);
		else if (checkCaracter(line, '}'))
		{
			initializeLocation();
			return;
		}
		else
			parseRule(line);
	}
	throw std::invalid_argument("no closing brace in location block!");
}

location::location(const location &rhs)
{
	std::cout << GREEN << "location: Copy constructor called" RESET << std::endl;
	*this = rhs;
}

location &
location::operator=(const location &rhs)
{
	// std::cout << GREEN << "location: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		 _alias = rhs._alias;
		_allow_methods = rhs._allow_methods;
		_autoindex = rhs._autoindex;
		_cgi_extension = rhs._cgi_extension;
		_cgi_path = rhs._cgi_path;
		_index = rhs._index;
		_return = rhs._return;
		_root = rhs._root;
		_nestedLocations = rhs._nestedLocations;
		_rulemap = rhs._rulemap;
		_name = rhs._name;
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

void	location::setRoot(const std::string &root)
{
	this->_root = root;
}

std::string	location::getAlias()
{
	return(this->_alias);
}

void	location::setAlias(const std::string &alias)
{
	this->_alias = alias;
}

std::vector<Http_method>	location::getAllowMethods()
{
	return(this->_allow_methods);
}

void	location::setAllowMethods(const std::vector<Http_method>  &allow_methods)
{
	this->_allow_methods = allow_methods;
}

bool	location::getAutoindex()
{
	return(this->_autoindex);
}

void	location::setAutoindex(const bool &autoindex)
{
	this->_autoindex = autoindex;
}

std::vector<std::string>	location::getCgiExtension()
{
	return(this->_cgi_extension);
}

void	location::setCgiExtension(const std::vector<std::string> &cgi_extension)
{
	this->_cgi_extension = cgi_extension;
}

std::vector<std::string>	location::getCgiPath()
{
	return(this-> _cgi_path);
}

void	location::setCgiPath(const std::vector<std::string> &cgi_path)
{
	this-> _cgi_path = cgi_path;
}

std::vector<std::string>	location::getIndex()
{
	return(this->_index);
}

void	location::setIndex(const std::vector<std::string> &index)
{
	this->_index = index;
}

std::string	location::getReturn()
{
	return(this->_return);
}

void	location::setReturn(const std::string &t_return)
{
	this->_return = t_return;
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //

std::string location::toString() const 
{
    std::ostringstream oss;
	oss << "Alias: " << _alias << "\n";
	oss << "Autoindex: " << _autoindex << "\n";
	oss << "Return: " << _return << "\n";
	oss << "Root: " << _root << "\n";
    return oss.str();
}

void	location::initializeLocation()
{
	setAlias(validateAlias());
	setAllowMethods(validateAllowMethods());
	setAutoindex(validateAutoindex());
	setCgiExtension(validateCgiExt());
	setCgiPath(validateCgiPath());
	setIndex(validateIndex());
	setReturn(validateReturn());
	setRoot(validateRoot());
}

void   location::parseRule(const std::string &line)
{
	auto comment_pos = line.find('#');
	std::string directive = line.substr(0, comment_pos);

	auto key_begin = directive.begin();
	while (key_begin != directive.end() and (*key_begin == ' ' or *key_begin == '\t'))
		key_begin++;
	if (key_begin == directive.end() or *key_begin == '#' or *key_begin == '}')
		return;
	auto key_end = key_begin;
	while (key_begin != directive.end() and *key_begin != ' ' and *key_begin != '\t')
		key_begin++;
	std::string tmp_key(key_end, key_begin);
	auto value_begin = key_end;
	while(value_begin!= directive.end() and (*value_begin== ' ' or+ *value_begin== '\t'))
	 	value_begin++;
	auto value_end = value_begin;
	while(value_end != directive.end() and *value_end != ';')
	 	value_end++;
	if (value_end == directive.end())
		throw std::invalid_argument("Error: Missing semicolon.");
	std::string tmp_value(value_begin, value_end);	
	_rulemap.emplace(tmp_key, tmp_value);
}