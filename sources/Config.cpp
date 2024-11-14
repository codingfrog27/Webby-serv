/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/14 16:07:01 by asimone          ###   ########.fr       */
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
			setMaxBodySize(validateMaxBodySize());
		if (_rulemap.contains("error_page"))
			setErrorPage(validateErrorPage());
		if (_rulemap.contains("host"))
			setHost(validateHost());
		if (_rulemap.contains("index"))
			setIndex(ValidateIndex());
		if (_rulemap.contains("root"))
			setRoot(validateRoot());
		if (_rulemap.contains("server_name"))
			setServerName(validateServerName());
	return (1);
}

std::string Config::toString() const {
    std::ostringstream oss;
    oss << "Server Name: " << _serverName << "\n";
	oss << "Root: " << _rootDir << "\n";
	oss << "Listen: " << _listen << "\n";
	oss << "Host: " << _host << "\n";
	// oss << "Error Page: " << _errorPage << "\n";
	// print_map(_errorPage);
	oss << "Max Body Size: " << _client_max_body_size << "\n";
	// oss << "Index: " << _index << "\n";
    return oss.str();
}

void	Config::parseRule(const std::string &line)
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
