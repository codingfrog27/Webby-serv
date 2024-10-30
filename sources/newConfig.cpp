/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:24:07 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/29 16:43:29 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::vector<Config>	parseConfigFile(const std::string fileName)
{
	std::string		line;
	std::ifstream	file(fileName);
	std::vector<Config>	Configs;

	if (!file.is_open())
		throw std::invalid_argument("Error: Unable to open file" );
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#')
			continue;
		if (line.find("server {") != std::string::npos) 
			Configs.emplace_back(file, line);
		else
			throw std::invalid_argument("non comment text between server blocks! >:(");
	}
	return (Configs);
}

Config::Config(std::ifstream &file, std::string &line)
{
	size_t i = 0;
	std::string set(" \t");
	while (std::getline(file, line))
	{
		// while (checkCaracter(set, line[i]))
		// 	i++;
		if (line.empty() || line[i] == '#')
			continue;
		// std::cout << line << "||" << std::endl;
		if (locationFound(line))
		{
			// _locNames.emplace_back(findLocationName); //2 vecs can be smushed into map if really wanted
			// _newLocations.emplace_back(file, line);
			location newloc(file, line);
			_newLocations.push_back(std::move(newloc));
		}
		if (checkCaracter(line, '}'))
		{
			for (const auto& pair : _rulemap) {
        	std::cout << pair.first << ": " << pair.second << std::endl;
			}
			// parseMap();
			return;
		}
		parseRule(line);
	}
}

bool	locationFound(std::string &line)
{
	if (checkstr(line, "location") && checkCaracter(line, '{')) //add error protect by checking till comment
	{
		// if (!checkCaracter(line, '{'))
		// {
		// 	throw (std::invalid_argument("no opening brace after location\n"));
		// }
		return (true);
	}
	return (false);
}

location::location(std::ifstream &file, std::string &line)
{
	_name = findLocationName(line);
	while (std::getline(file, line)) 
	{
		if (line.empty() || line[0] == '#')
		continue;
		if (checkCaracter(line, '{'))
		{
			//can have a nested loc vec just like in config if wanted
			location newloc(file, line);
			_nestedLocations.push_back(std::move(newloc));
			// _nestedLocations.emplace_back(file, &line);
		}
		if (checkCaracter(line, '}'))
		{
			for (const auto& pair : _rulemap) {
        	std::cout << pair.first << ": " << pair.second << std::endl;
			return;
		}
		}
		parseRule(line);
}
}

void	Config::parseRule(const std::string &line)
{
	//just cpd from u w map
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
	// _rulemap.insert(tmp_key, tmp_value);
	_rulemap[tmp_key] = tmp_value;
	
}

void	location::parseRule(const std::string &line) //im lazy, either config inherits from
{ // location or mb better make this standalone function that returns a pair
	//just cpd from u w map
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
    	return; //exception
	std::string tmp_value(value, begin_value);	
	// _rulemap.insert(tmp_key, tmp_value);
	_rulemap[tmp_key] = tmp_value;
}


bool	checkstr(const std::string &line, const std::string &str)
{
	if (line.find(str) != std::string::npos)
		return (true);
	return (false);
}
