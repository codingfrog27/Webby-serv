/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/02/05 18:51:43 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "Config.hpp"
#include "socket.hpp"
#include <memory>
#include <vector>
#include <set>


void	parseConfig(int argc, char ** argv, std::vector<Config> &configs)
{
	try
	{
		if (argc != 2)
			throw Config::NoBlockFound("please oprovide a config file (and no other arguments)");
		configs = readConfigFile(argv[1]);
		// printConfigs(configs);
	}
	catch(const Config::NoBlockFound &e) //general config error
	{
		std::cerr << e.what() << "\n running with default config values" << std::endl;
		configs.emplace_back();
	}
	catch (const std::exception &e)
	{
		std::cerr << RED "Config error found: " << e.what() << " \n\nclosing server" RESET << std::endl;
		exit(1);
	}
}


std::vector<Config>	readConfigFile(const std::string fileName)
{
	std::vector<Config>	Configs;
	std::string			line;
	std::ifstream		file(fileName);

	if (!file.is_open())
		throw std::invalid_argument("Error: Unable to open file" );
	while (std::getline(file, line))
	{
		if (line.empty() || checkCaracter(line, '#'))
			continue;
		if (line.find("server {") != std::string::npos)
			Configs.emplace_back(file, line);
		// else
			// throw std::invalid_argument("non comment text between server blocks! >:(");
	}
	if (Configs.empty())
		throw Config::NoBlockFound("no server blocks found! be sure to start your block wit \"server {\"");
	// checkPortUniqueness(Configs);
	return (Configs);
}

Config::Config(std::ifstream &file, std::string &line)
{
	std::cout << GREEN << "config filestream constructor called" \
	<< RESET << std::endl;
	readBlock(file, line);
}

std::string	extractLocationName(std::string line)
{
	std::string location = normalize_space_location(line);
	std::string location_name;

    size_t startPos = location.find("/");
    if (startPos != std::string::npos) 
	{
        size_t endPos = location.find_first_of(" {", startPos);
        if (endPos == std::string::npos) 
            endPos = location.length();
    location_name = location.substr(startPos, endPos - startPos);
	}
	// std::cout << location_name << std::endl;
	return (location_name);
}


void	Config::readBlock(std::ifstream &file, std::string &line)
{
	while (std::getline(file, line))
	{
		if (line.empty() || line[i] == '#')
			continue;
		if (locationFound(line))
		{
			std::string location_name = extractLocationName(line);
			_locations.emplace_back(file, line);
		}
		else if (checkCaracter(line, '}'))
		{
			mapToMembers();
			return;
		}
		else
			parseRule(line);
	}
	throw std::invalid_argument("no closing brace in server block!");
}

void Config::mapToMembers()
{
		std::vector<std::string> serverBlock {"autoindex", "client_max_body_size", "error_page", "host", "index", "listen", "root", "server_name", "timeout"};
		std::set<std::string> uniqueKeys;
		
		for (const auto& [key, value] : _rulemap)
			uniqueKeys.insert(key);
		
		std::vector<std::string> keys(uniqueKeys.begin(), uniqueKeys.end());
		
		std::sort(keys.begin(), keys.end());
		serverBlock.erase(std::unique(serverBlock.begin(), serverBlock.end()), serverBlock.end());
		//check if keys match the expected serverBlock configuration
		if (keys == serverBlock)
		{
			_autoindex = validateAutoindex(); 
			_listen = validateListen(); 
			_client_max_body_size = validateMaxBodySize(); 
			_errorPage = validateErrorPage();
			_host = validateHost();
			_index = ValidateIndex();
			_rootDir = validateRoot();
			_timeout = validateTimeout(); 
			_serverName = validateServerName(); 
		}
		else
			throw std::invalid_argument("Keys do not match the expected serverBlock configuration");
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
	_rulemap.emplace(tmp_key, tmp_value);
}
