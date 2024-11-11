/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/11 14:11:34 by asimone          ###   ########.fr       */
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
	std::cout << GREEN << "config filestream constructor called" \
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
			
			return;
		}
		else
			parseRule(line);
	}
}


int	Config::mapToMembers()
{		
		if (_rulemap.contains("listen"))
			setListen(validateListen());
		if (_rulemap.contains("client_max_body_size"))
			setMaxBodySize("client_max_body_size");
		if (_rulemap.contains("error_page"))
			setErrorPage("error_page");
		if (_rulemap.contains("host"))
			setHost("host");
		if (_rulemap.contains("index"))
			setIndex("index");
		if (_rulemap.contains("root"))
			setRoot("root");
		if (_rulemap.contains("server_name"))
			setServerName("server_name");
	return (1);
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