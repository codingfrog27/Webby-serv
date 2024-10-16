/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/16 15:37:29 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <algorithm>
#include "socket.hpp"
#include "Colors.hpp"


// ************************************************************************** //
//						Constructors and Destructors						//
// ************************************************************************** //

Config::Config(void) //default constructor 
{
	_serverPort = "8080"; //idk if this works 
	_serverName = "localhost"; //"funny-server";
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
//								Public methods							  //
// ************************************************************************** //

bool	checkCaracter(const std::string &line, const char &c)
{
	if (line.find(c) != std::string::npos)
		return (true);
	return (false);
}

std::string removeSpaces(const std::string &str)
{
	std::string result;
	result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
	return (result);
}


void Config::parseConfigFile(const std::string fileName)
{
	std::string			line;
	std::ifstream		file(fileName);
	static size_t inServerBlock = 0;
    static size_t inLocationBlock = 0;
	

	if (!file.is_open())
	{
		std::cerr << "Error: Unable to open file" << std::endl;
		return ;
	}
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
        	if (line.empty() || line[0] == '#')
        	    continue;
			
        	if (line.find("server {") != std::string::npos) 
			{
				inServerBlock++;
        		std::cout << "Entering server block" << std::endl;
        	    continue;
        	}
        	if (inServerBlock) 
			{	
        	    if (line.find("location") != std::string::npos && checkCaracter(line, '{')) 
				{
					auto location_name = line.begin();
					while (*location_name == ' ' or *location_name == '\t')
						location_name++;
					auto begin = location_name;
					while (*location_name != ' ' and *location_name != '\t')
						location_name++;
					
					auto location_value = location_name;
					while (*location_value == ' ' or *location_value == '\t')
						location_value++;
					auto begin_value = location_value;
					while(*location_value != ' ' and *location_value != '\t')
						location_value++;
					std::string tmp_value(begin_value, location_value);
					_locationName.push_back(tmp_value);
					std::cout << _locationName.back() << std::endl;
					
					inLocationBlock++;
        	        continue;
        	    }
				if (inLocationBlock != 0)
					parseLocationBlock(line);
        	    if (inLocationBlock) 
				{
        	        if (checkCaracter(line, '}')) 
					{
        	            inLocationBlock--;
        	            // std::cout << "Exiting location block" << std::endl;
        	        }
        	        continue;
        	    }
        	    if (checkCaracter(line, '}')) 
				{
					if (inServerBlock == 1)
						inServerBlock--;
        	        // std::cout << "Exiting server block" << std::endl;
        	    } 
				else 
				{
        	        parseServerBlock(line);
        	        // std::cout << "Parsing line in server block: " << line << std::endl;
        	    }
        	}
    	}
	file.close();
	}
	else 
		std::cerr << "Error: Unable to open file" << std::endl;
return ;
}


int	Config::parseServerBlock(const std::string &line)
{
	std::map<std::string, std::string> serverBlock;

	if (line.empty())
		return (0);
	auto key = line.begin();
	while (key != line.end() and (*key == ' ' or *key == '\t'))
		key++;
	if (key == line.end() or *key == '#' or *key == '}')
		return (0);
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
	std::string tmp_value(value, begin_value);
	serverBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));

	for (const auto& pair : serverBlock) 
        std::cout << pair.first << ": " << pair.second << std::endl;
	return (0);
}



int	Config::parseLocationBlock(const std::string &line)
{
	std::map<std::string, std::string> locationBlock;

	if (line.empty())
		return (0);
	auto key = line.begin();
	while (key != line.end() and (*key == ' ' or *key == '\t'))
		key++;
	if (key == line.end() or *key == '#' or *key == '}')
		return (0);
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
	std::string tmp_value(value, begin_value);
	locationBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));

	for (const auto& pair : locationBlock) 
        std::cout << pair.first << ": " << pair.second << std::endl;
	return (0);
}