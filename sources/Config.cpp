/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/23 17:24:26 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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

std::string	findLocationName(const std::string &line)
{
	auto location_name = line.begin();
	while (location_name != line.end() && (*location_name == ' ' or *location_name == '\t'))
		location_name++;
	auto begin = location_name;
	while (location_name != line.end() && (*location_name != ' ' and *location_name != '\t'))
		location_name++;
	
	auto location_value = location_name;
	while (location_value != line.end() && (*location_value == ' ' or *location_value == '\t'))
		location_value++;
	auto begin_value = location_value;
	while(location_value != line.end() && (*location_value != ' ' and *location_value != '\t'))
		location_value++;

	return (std::string(begin_value, location_value));
}

void	Config::findKeyandValue(const std::string &line, std::multimap<std::string, std::string> &block)
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
	block.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));
}

Config* Config::parseConfigFile(const std::string fileName)
{
	Config			*configfile = new Config();
	std::string		line;
	std::ifstream	file(fileName);
	static size_t 	inServerBlock = 0;
    static size_t 	inLocationBlock = 0;

	if (!file.is_open())
	{
		std::cerr << "Error: Unable to open file" << std::endl;
	}
	try
	{	
		int i = 0;
		while (std::getline(file, line))
		{
    		if (line.empty() || line[0] == '#')
    		    continue;
			
    		if (line.find("server {") != std::string::npos) 
			{
				inServerBlock++;
    			// std::cout << "Entering server block" << std::endl;

				//make config/serverblock object here and loop further in the object (by passing the stream)
    		    continue;
    		}
    		if (inServerBlock) 
			{	
    		    if (line.find("location") != std::string::npos && checkCaracter(line, '{')) 
				{
					// std::cout << line << std::endl;
					//make location object here and loop further in the object (by passing the stream)
					//vec.emplaceback(infile)

					std::string tmp_value(findLocationName(line));	
					// _locationName.push_back(tmp_value);
					parseLocationBlock(file, tmp_value);
					// std::cout << _locationName.back() << std::endl;
    		        continue;
    		    }
    		    // if (inLocationBlock) 
				// {
    		    // if (checkCaracter(line, '}')) 
				// {
				// 	if (inServerBlock == 1)
				// 		inServerBlock--;
    		           // inLocationBlock--;
    		           // std::cout << "Exiting location block" << std::endl;
    		    // }
				else 
				{
					std::cout << line << std::endl;
    			    parseServerBlock(line, configfile);
				}    
    		       // continue;
    		    	// std::cout << "Parsing line in server block: " << line << std::endl;
			}
    	}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		delete configfile;
		throw;
	}
	return (configfile);
}

void	Config::parseServerBlock(const std::string &line, Config *configFile)
{
	int nonCommentLines = 0;

	try
	{
		nonCommentLines++;
		//if (line.empty())
		//	throw (std::invalid_argument("Error: Empty line"));	
		findKeyandValue(line, configFile->serverBlock);
		if (nonCommentLines == 0)
			throw (std::invalid_argument("Error: Empty line"));
		// printBlockValue(configFile->serverBlock);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return ;
}



void	Config::parseLocationBlock(std::ifstream &file, const std::string &locationName)
{
	std::string line;
	location	loc;
	int			locationNested = 1;

	try
	{	
		while (std::getline(file, line)) 
		{
        	if (line.empty() || line[0] == '#')
            	continue;
			if (checkCaracter(line, '{')) 
             	locationNested++;
			if (checkCaracter(line, '}'))
			{
				locationNested--;
				if (locationNested == 0)
             		break;
			}
			if (locationNested == 1)
				findKeyandValue(line, loc.locationBlock);
			else if (locationNested > 1 && line.find("location") != std::string::npos)
			{
				std::string nestedLocation(findLocationName(line));
				parseLocationBlock(file, nestedLocation);
			}
        }
		_locations.insert(std::make_pair(locationName, loc));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return;
}

// std::string Config::updateValue(const std::string& valueToFind, Config &config) 
// {
// 	bool valueFound = false;
//     auto range = config.serverBlock.equal_range(valueToFind);
	
//     for (auto it = range.first; it != range.second; ++it)
// 	{
//         // _serverPort = it->second;
// 		valueFound = true;
// 		return (it->second);
// 		// break;
// 	}
// 	if (!valueFound)
// 		return ("");	
// 		// _serverPort = "0";
// }


// void	Config::mapToMembers()
// {
// 	// char *rules = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

// 	// for (size_t i = 0; rules[i]; i++)
// 	// {
// 	// 	/* code */
// 	// }

// 	bool check;

// 	_timeout = updateListenValue("timeout", *this);
// 	if (_timeout == "")
// 		_timeout = "5000";
// 	_rootDir = updateListenValue("root", *this);
	
	
// }
void	Config::printBlockValue(const std::multimap<std::string, std::string> &configBlock)
{
	for (const auto& pair : configBlock)
		std::cout << pair.first << ": " << pair.second << std::endl;	
}

void Config::printLocationBlock() const 
{
    for (const auto& pair : _locations) {
        std::cout << "Location: " << pair.first << std::endl;
        for (const auto& setting : pair.second.locationBlock) {
            std::cout << "  " << setting.first << ": " << setting.second << std::endl;
        }
    }
}