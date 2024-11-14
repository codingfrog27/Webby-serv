/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/14 12:48:38 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "Colors.hpp"
#include <sstream>

// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

location::location(void)
{
	std::cout << GREEN << "location: Default constructor called" << RESET << std::endl;
}

location::location(std::ifstream &file, std::string &line)
{
	std::cout << GREEN << "location filestream constructor called" \
	<< RESET << std::endl;
	size_t i = 0;
	while (std::getline(file, line))
	{
		if (line.empty() || line[i] == '#')
			continue;
		if (locationFound(line))
			_nestedLocations.push_back(std::unique_ptr<location>(new location(file, line)));
		else if (checkCaracter(line, '}'))
		{
			initializeLocation();
			return;
		}
		else
			parseRule(line);
	}
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

std::string	location::getIndex()
{
	return(this->_index);
}

void	location::setIndex(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_index = _rulemap.at(key);
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

std::string location::toString() const 
{
    std::ostringstream oss;
	oss << "Alias: " << _alias << "\n";
	oss << "Allow_methods: " << _allow_methods << "\n";
	oss << "Autoindex: " << _autoindex << "\n";
	oss << "Cgi_extension: " << _cgi_extension << "\n";
	oss << "Cgi path: " << _cgi_path << "\n";
	oss << "Index: " << _index << "\n";
	oss << "Return: " << _return << "\n";
	oss << "Root: " << _root << "\n";
    return oss.str();
}

void	location::initializeLocation()
{
	setAlias("alias ");
	setAllowMethods("allow_methods ");
	setAutoindex("autoindex ");
	setCgiExtension("cgi_ext ");
	setCgiPath("cgi_path ");
	setIndex("index ");
	setReturn("return ");
	setRoot("root ");
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
	// std::cout << tmp_value << std::endl;
	_rulemap.emplace(tmp_key, tmp_value);
}