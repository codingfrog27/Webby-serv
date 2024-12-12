/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/12 13:59:00 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "socket.hpp"
#include <memory>
#include <vector>
#include <set>

std::vector<std::unique_ptr<Config>>	parseConfigFile(const std::string fileName)
{
	std::string		line;
	std::ifstream	file(fileName);
	std::vector<std::unique_ptr<Config>>	Configs;

	if (!file.is_open())
		throw std::invalid_argument("Error: Unable to open file" );
	while (std::getline(file, line))
	{
		if (line.empty() || checkCaracter(line, '#'))
			continue;
		if (line.find("server {") != std::string::npos)
		{
			Configs.push_back(std::unique_ptr<Config>(new Config(file, line)));
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

int Config::mapToMembers()
{
	try 
	{
		std::vector<std::string> serverBlock {"client_max_body_size", "error_page", "host", "index", "listen", "root", "server_name"};
		std::set<std::string> uniqueKeys;
		
		for (const auto& [key, value] : _rulemap)
			uniqueKeys.insert(key);
		
		std::vector<std::string> keys(uniqueKeys.begin(), uniqueKeys.end());
		
		std::sort(keys.begin(), keys.end());
		std::sort(serverBlock.begin(), serverBlock.end());
		serverBlock.erase(std::unique(serverBlock.begin(), serverBlock.end()), serverBlock.end());
	
		if (keys == serverBlock)
		{
			try 
			{
				setListen(validateListen()); 
			}
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setListen: " << e.what() << std::endl;
				return (-1);
			}

			try 
			{
				setMaxBodySize(validateMaxBodySize());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setMaxBodySize: " << e.what() << std::endl;
				return (-2);
			}

			try 
			{
				setErrorPage(validateErrorPage());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setErrorPage: " << e.what() << std::endl;
				return (-3);
			}

			try 
			{
				setHost(validateHost());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setHost: " << e.what() << std::endl;
				return (-4);
			}

			try 
			{
				setIndex(ValidateIndex());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setIndex: " << e.what() << std::endl;
				return (-5);
			}

			try 
			{
				setRoot(validateRoot());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setRoot: " << e.what() << std::endl;
				return (-6);
			}

			try 
			{
				setServerName(validateServerName());
			} 
			catch (const std::exception& e) 
			{
				std::cerr << "Error in setServerName: " << e.what() << std::endl;
				return (-7);
			}
		}
		else
		{
			std::cerr << "Keys do not match the expected serverBlock configuration" << std::endl;
			return (0);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected error in mapToMembers: " << e.what() << std::endl;
		return (-8);
	}

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
	_rulemap.insert(make_pair(tmp_key, tmp_value));
}
