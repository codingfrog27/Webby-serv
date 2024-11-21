/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_obj_utils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:35:05 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/21 17:01:08 by mde-cloe         ###   ########.fr       */
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
	_maxConnects = 1;
	_timeout = 50000;
	_rootDir = "website/";
	_autoIndexFilePath = _rootDir + "index.html"; //do i need slash?
	

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
		_serverPort = rhs._serverPort;
		_serverName = rhs._serverName;
		_maxConnects = rhs._maxConnects;
		_timeout = rhs._timeout;
		_rootDir = rhs._rootDir;
		_autoIndexFilePath = rhs._autoIndexFilePath;
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

std::string location::getName() const { //location cpp?
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

std::unordered_map<std::string, std::string>	Config::getErrorPage()
{
	return(this->_errorPage);
}

void	Config::setErrorPage(const std::unordered_map<std::string, std::string> &errorPageMap)
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