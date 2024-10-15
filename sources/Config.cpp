/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/15 16:17:42 by asimone          ###   ########.fr       */
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
	// std::cout << GREEN << "Parsing line in Server block: " << RESET;
	std::string			content;
	std::string			token;
	std::map<std::string, std::string> locationBlock;
	std::istringstream	ss(line);
	
	// check se finisce con il char ';'
	// find end of line (position of ';'), start checking from left to right
	// std::cout << "This is line: " << line << std::endl;
	// if (line.find(''))
	auto key = line.begin();
	while (*key == ' ' or *key == '\t')
		key++;
	auto begin = key;
	while (*key != ' ' and *key != '\t')
		key++;
	std::string tmp_key(begin, key);
		
	// size_t start = 0;
	// while (line[start] && isspace(line[start]))
	// 	start++;
	// std::string key, value;
	// std::string actual_start = line.substr(start);
	if (line.find('#') or line.size() == 0 or line.empty())
		return (2);
	auto value  = key;
	while(*value == ' ' or *value == '\t')
		value++;
	auto begin_value = value;
	while(*begin_value != ';')
		begin_value++;
	std::string tmp_value(value, begin_value);
	locationBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));
	
	
	// auto key 
	// if (end_key.end() == std::string::npos)
	// {
		//errore config non valido
		// std::cout << "This is end_key: " << actual_start << std::endl;
		// std::cerr << RED << "1) Error: Invalid config file" << RESET << std::endl;
		// stop parsing throw error ... 
	// }
	// else
	// {
	// std::cout << "This is line: " << line << std::endl;
		// key = actual_start.substr(0, end_key);
		// // std::cout << "This is key: " << key << std::endl;
		// size_t start_value = end_key;
		// while (actual_start[start_value] and isspace(actual_start[start_value]))
		// 	start_value++;
		// // std::cout << actual_start.substr(start_value) << '\n';
		// size_t n_chars = 0;
		// while (actual_start[start_value + n_chars] and
		//  	actual_start[start_value + n_chars] != ';')
		// 	n_chars++;
		// value = actual_start.substr(start_value, n_chars);
		// std::cout << key << " - " << value << std::endl;
	// }
	if (!checkCaracter(line, '}') && !checkCaracter(line, ';'))
		return (1);
	return (0);
}



int	Config::parseLocationBlock(const std::string &line)
{
	// std::string			content;
	// std::string			token;
	// std::istringstream	ss(line);
	// // std::cout << CYAN << "Parsing line in Location block: " << RESET;
	
	// // check se finisce con il char ';'
	// // find end of line (position of ';'), start checking from left to right
	// // std::cout << "This is line: " << line << std::endl;
	// // if (line.find(''))
	
	// size_t start = 0;
	// while (line[start] && isspace(line[start]))
	// 	start++;
	// std::string key, value;
	// std::string actual_start = line.substr(start);
	// if (actual_start[0] == '#' or actual_start.size() == 0 or actual_start.empty())
	// 	return (2);
	// size_t end_key = actual_start.find(' ');
	// if (end_key == std::string::npos)
	// {
	// 	//errore config non valido
	// 	// std::cout << "This is end_key: " << actual_start << std::endl;
	// 	// std::cerr << RED << "1) Error: Invalid config file" << RESET << std::endl;
	// 	// stop parsing throw error ... 
	// }
	// else
	// {
	// // std::cout << "This is line: " << line << std::endl;
	// 	key = actual_start.substr(0, end_key);
	// 	// std::cout << "This is key: " << key << std::endl;
	// 	size_t start_value = end_key;
	// 	while (actual_start[start_value] and isspace(actual_start[start_value]))
	// 		start_value++;
	// 	// std::cout << actual_start.substr(start_value) << '\n';
	// 	size_t n_chars = 0;
	// 	while (actual_start[start_value + n_chars] and
	// 	 	actual_start[start_value + n_chars] != ';')
	// 		n_chars++;
	// 	value = actual_start.substr(start_value, n_chars);
	// 	std::cout << key << " - " << value << std::endl;
	// }
	std::map<std::string, std::string> locationBlock;

	auto key = line.begin();
	while (*key == ' ' or *key == '\t')
	{
		if (*key == '#')
			continue;
		key++;
	}
	auto begin = key;
	while (*key != ' ' and *key != '\t')
		key++;
	std::string tmp_key(begin, key);
	std::cout << tmp_key << std::endl;
		
	// if (line.find('#') or line.size() == 0 or line.empty())
	// 	return (2);
	// size_t start = 0;
	// while (line[start] && isspace(line[start]))
	// 	start++;
	// std::string key, value;
	// std::string actual_start = line.substr(start);
	auto value  = key;
	while(*value == ' ' or *value == '\t')
		value++;
	auto begin_value = value;
	while(*begin_value != ';')
		begin_value++;
	std::string tmp_value(value, begin_value);
	// locationBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));

	// for (const auto& pair : locationBlock) {
    //     std::cout << pair.first << ": " << pair.second << std::endl;
    // }
	
	if (!checkCaracter(line, '}') && !checkCaracter(line, ';'))
		return (1);
	return (0);
}