/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/06 19:46:06 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "socket.hpp"
#include <memory>

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

Config::Config(std::ifstream &file, std::string &line)
{
	std::cout << GREEN << "location filestream constructor called" \
	<< RESET << std::endl;
	while (std::getline(file, line))
	{
		if (line.empty() || line[i] == '#')
			continue;
		if (locationFound(line))
			_newLocations.push_back(std::unique_ptr<location>(new location(file, line)));
		else if (checkCaracter(line, '}'))
		{
			int validServer = mapToMembers();
			setServer(validServer);
			// if (validServer == EMPTY)
			// 	Config defaultConfig;				
			// else 
			return;
		}
		else
			parseRule(line);
	}
}


int	Config::mapToMembers()
{
	int rule = EMPTY;
	int	counter = 0;
	
	for (auto i = _rulemap.begin(); i != _rulemap.end(); i++)
	{			
		std::cout << counter << std::endl;	
		counter++;
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
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case MAX_BODY_SIZE:
		setMaxBodySize("client_max_body_size");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case ERROR_PAGE:
		setErrorPage("error_page");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case HOST:
		setHost("host");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case INDEX:
		setIndex("index");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case ROOT:
		setRoot("root");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case SERVER_NAME:
		setServerName("server_name");
		if (_rulemap.empty())
			break;
		else
			mapToMembers();
	case EMPTY:
		std::cout << "hello gamers" << std::endl;
		break;
	default:
		throw std::invalid_argument("Error: Invalid rule");
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