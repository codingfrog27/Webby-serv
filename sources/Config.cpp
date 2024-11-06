/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/06 16:57:36 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "socket.hpp"
#include "Colors.hpp"
#include <memory>

enum ServerRules
{
	LISTEN,
	MAX_BODY_SIZE,
	ERROR_PAGE,
	HOST,
	INDEX,
	ROOT,
	SERVER_NAME,
	EMPTY
};

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
		// Perform deep copy
	}

	return (*this);
}

Config::~Config(void)
{
	std::cout << RED << "Config: Destructor called" << RESET << std::endl;
}

// ************************************************************************** //
//								GETTER & SETTER								  //
// ************************************************************************** //

std::string	Config::getRoot()
{
	return(this->_rootDir);
}

void	Config::setRoot(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_rootDir = _rulemap.at(key);
}

std::string	Config::getListen()
{
	return(this->_listen);
}

void	Config::setListen(const std::string &value)
{
	// if (!_rulemap.contains(key))
	// 	return;
	this->_listen = value;
}

std::string	Config::getServerName()
{
	return(this->_serverName);
}

void	Config::setServerName(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_serverName = _rulemap.at(key);
}

std::string	Config::getHost()
{
	return(this->_host);
}

void	Config::setHost(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_host = _rulemap.at(key);
}

std::string	Config::getErrorPage()
{
	return(this->_errorPage);
}

void	Config::setErrorPage(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_errorPage = _rulemap.at(key);
}

std::string	Config::getMaxBodySize()
{
	return(this->_client_max_body_size);
}

void	Config::setMaxBodySize(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_client_max_body_size = _rulemap.at(key);
}

std::string	Config::getIndex()
{
	return(this->_index);
}

void	Config::setIndex(const std::string &key)
{
	if (!_rulemap.contains(key))
		return;
	this->_index = _rulemap.at(key);
}

// ************************************************************************** //
//								Public methods							  //
// ************************************************************************** //

bool	checkCaracter(const std::string &line, const char &c)
{
	if (line.find(c) != std::string::npos)
		return (true);
	return (false);
}

bool	checkstr(const std::string &line, const std::string &str)
{
	if (line.find(str) != std::string::npos)
		return (true);
	return (false);
}

bool	locationFound(std::string &line)
{
	if (checkstr(line, "location") && checkCaracter(line, '{'))
		return (true);
	return (false);
}

bool	endMap(std::unordered_map<std::string, std::string> &map)
{
	// for(auto i = map.begin(); i != map.end(); i++)
	// {
	// 	if (i == map.end())
	// 		return (true);
	// }
	// return (false);
	return (map.empty());
}

std::vector<Config>	parseConfigFile(const std::string fileName)
{
	std::string		line;
	std::ifstream	file(fileName);
	std::vector<Config>	Configs;

	if (!file.is_open())
		throw std::invalid_argument("Error: Unable to open file" );
	while (std::getline(file, line))
	{
		if (line.empty() || checkCaracter(line, '#'))
			continue;
		if (line.find("server {") != std::string::npos)
		{
			Configs.emplace_back(file, line);
		}
		// else
			// throw std::invalid_argument("non comment text between server blocks! >:(");
	}
	return (Configs);
}

std::string location::getName() const {
    return _name;
}

Config::Config(std::ifstream &file, std::string &line)
{
	size_t i = 0;
	while (std::getline(file, line))
	{
		if (line.empty() || line[i] == '#')
			continue;
		if (locationFound(line))
			_newLocations.push_back(std::unique_ptr<location>(new location(file, line)));
		else if (checkCaracter(line, '}'))
		{
			int validServer = initializeServer();
			if (validServer == EMPTY)
			{
				Config defaultConfig();
				
			}
			else 
				setServer(validServer);
			return;
		}
		else
			parseRule(line);
	}
}

std::string Config::toString() const {
    std::ostringstream oss;
    oss << "Server Name: " << _serverName << "\n";
	oss << "Root: " << _rootDir << "\n";
	oss << "Listen: " << _listen << "\n";
	oss << "Host: " << _host << "\n";
	oss << "Error Page: " << _errorPage << "\n";
	oss << "Max Body Size: " << _client_max_body_size << "\n";
	oss << "Index: " << _index << "\n";
    return oss.str();
}

std::string	Config::validateListen()
{
	std::string listen;
	if (!_rulemap.contains("listen"))
		throw std::invalid_argument("Error: listen directive not found");
	listen = _rulemap.at("listen");
	return (listen);
}

int	Config::initializeServer()
{
	int rule = EMPTY;
	
	for (auto i = _rulemap.begin(); i != _rulemap.end(); i++)
	{				
		if (_rulemap.contains("listen"))
			rule = LISTEN;
		else if (_rulemap.contains("client_max_body_size"))
			rule = MAX_BODY_SIZE;
		else if (_rulemap.contains("error_page"))
			rule = ERROR_PAGE;
		else if (_rulemap.contains("host"))
			rule = HOST;
		else if (_rulemap.contains("index"))
			rule = INDEX;
		else if (_rulemap.contains("root"))
			rule = ROOT;
		else if (_rulemap.contains("server_name"))
			rule = SERVER_NAME;
		else
			rule = EMPTY;
	}
	return (rule);
}

void	Config::setServer(const int rule)
{
	switch (rule)
	{
	case LISTEN:
		setListen(validateListen());
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case MAX_BODY_SIZE:
		setMaxBodySize("client_max_body_size");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case ERROR_PAGE:
		setErrorPage("error_page");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case HOST:
		setHost("host");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case INDEX:
		setIndex("index");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case ROOT:
		setRoot("root");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	case SERVER_NAME:
		setServerName("server_name");
		if (endMap(_rulemap))
			break;
		else
			initializeServer();
	// default:
	// 	throw std::invalid_argument("Error: Invalid rule");
	}
}

void	Config::parseRule(const std::string &line)
{
	auto key = line.begin();
	while (key != line.end() and (*key == ' ' or *key == '\t'))
		key++;
	if (key == line.end() or *key == '#' or *key == '}')
		return;
	auto begin = key;
	while (key != line.end() and *key != ' ' and *key != '\t')
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
}

location::location(std::ifstream &file, std::string &line)
{
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