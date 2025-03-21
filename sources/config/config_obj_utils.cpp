/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_obj_utils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:35:05 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/02/05 18:28:43 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Colors.hpp"

// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Config::Config(void) //default constructor 
{
	_listen = "8080"; //idk if this works 
	_serverName = "localhost"; //"funny-server";
	_host = "127.0.0.1";
	_timeout = 50000;
	_rootDir = "website/";
	_autoIndexFilePath = _rootDir + "index.html";
	_autoindex = true;
	

	std::cout << GREEN << "Config: Default constructor called" << RESET << std::endl;
}

Config::Config(const Config &rhs)
{
	std::cout << GREEN << "Config: Copy constructor called" << RESET << std::endl;

	*this = rhs;
}

Config &
Config::operator=(const Config &rhs)
{
	std::cout << GREEN << "Config: Assignment operator called" << RESET << std::endl;

	if (this != &rhs)
	{
		_host = rhs._host;
		_autoIndexFilePath = rhs._autoIndexFilePath;
		_errorPage = rhs._errorPage;
		_client_max_body_size = rhs._client_max_body_size;
		_index = rhs._index;
		_listen = rhs._listen;
		_serverName = rhs._serverName;
		_timeout = rhs._timeout;
		_rootDir = rhs._rootDir;
		_autoindex = rhs._autoindex;
		_autoIndexFilePath = rhs._autoIndexFilePath;
		_errorPage = rhs._errorPage;
		_locations = rhs._locations;
		// _locationName = rhs._locationName;


	}

	return (*this);
}

Config::~Config(void)
{
	std::cout << RED << "Config: Destructor called" << RESET << std::endl;
}


// ************************************************************************** //
//								Get and setters								  //
// ************************************************************************** //

std::string &location::getName(){ //location cpp?
    return _name;
}

std::string	Config::getRoot()
{
	return(this->_rootDir);
}

void	Config::setRoot(const std::string &root)
{
	
	this->_rootDir = root;
}

std::string	Config::getListen()
{
	return(this->_listen);
}

void	Config::setListen(const std::string &listen)
{
	this->_listen = listen;
}

std::string	Config::getServerName()
{
	return(this->_serverName);
}

void	Config::setServerName(const std::string &serverName)
{
	this->_serverName = serverName;
}

std::string	Config::getHost()
{
	return(this->_host);
}

void	Config::setHost(const std::string &host)
{
	this->_host = host;
}

std::multimap<std::string, std::string>	Config::getErrorPage()
{
	return(this->_errorPage);
}

void	Config::setErrorPage(const std::multimap<std::string, std::string> &errorPageMap)
{
	this->_errorPage = errorPageMap;
}

std::string	Config::getMaxBodySize()
{
	return(this->_client_max_body_size);
}

void	Config::setMaxBodySize(const std::string &maxBodySize)
{
	this->_client_max_body_size = maxBodySize;
}

std::vector<std::string>	Config::getIndex()
{
	return(this->_index);
}

void	Config::setIndex(const std::vector<std::string> &index_vector)
{
	this->_index = index_vector;
}

size_t	Config::getTimeout()
{
	return(this->_timeout);
}

void	Config::setTimeout(const size_t& timeout)
{
	this->_timeout = timeout;
}

bool		Config::getAutoindex()
{
	return (this->_autoindex);
}

void Config::setAutoindex(const bool& autoIndex)
{
	this->_autoindex = autoIndex;
}

Config::NoBlockFound::NoBlockFound(std::string errMsg) : _errMsg(errMsg)
{}

const char		*Config::NoBlockFound::what() const noexcept
{
	return (_errMsg.c_str());
}