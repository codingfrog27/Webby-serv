/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/22 18:19:01 by asimone          ###   ########.fr       */
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

Config* Config::parseConfigFile(const std::string fileName)
{
	Config				*configfile = new Config();
	std::string			line;
	std::ifstream		file(fileName);
	static size_t inServerBlock = 0;
    static size_t inLocationBlock = 0;
	std::vector<Config> serverblocks;
	

	if (!file.is_open())
	{
		std::cerr << "Error: Unable to open file" << std::endl;
	}
	try
	{	
		int i = 0;
		while (std::getline(file, line))
		{
			std::cout << line << std::endl;
			i++;
			std::cout << i << std::endl;
    		if (line.empty() || line[0] == '#')
    		    continue;
			
    		if (line.find("server {") != std::string::npos) 
			{
				inServerBlock++;
    			// std::cout << "Entering server block" << std::endl;
    		    continue;
    		}
    		if (inServerBlock) 
			{	
    		    if (line.find("location") != std::string::npos && checkCaracter(line, '{')) 
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
					std::string tmp_value(begin_value, location_value);
					_locationName.push_back(tmp_value);
					// std::cout << _locationName.back() << std::endl;
					
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
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		delete configfile;
		throw;
	}
	return (configfile);
}


void	Config::parseServerBlock(const std::string &line)
{
	int nonCommentLines = 0;

	try
	{
		//if (line.empty())
		//	throw (std::invalid_argument("Error: Empty line"));
		auto key = line.begin();
		while (key != line.end() and (*key == ' ' or *key == '\t'))
			key++;
		if (key == line.end() or *key == '#' or *key == '}')
			return;
		nonCommentLines++;
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
		serverBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));	
		if (nonCommentLines == 0)
			throw (std::invalid_argument("Error: Empty line"));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return ;
}



void	Config::parseLocationBlock(const std::string &line)
{
	//if (line.empty())
	//	return (0);
	try
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
		locationBlock.insert(std::pair<std::string, std::string>(tmp_key, tmp_value));
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