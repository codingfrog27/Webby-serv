/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/09 15:52:23 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <algorithm>
#include "socket.hpp"
#include "Colors.hpp"


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

Config::Config(void) //default constructor 
{
	_serverPort = "8080"; //idk if this works 
	_serverName = "funny_serverUWU";
	_maxConnects = 500;
	_timeout = 50000;
	_rootDir = "website";
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
//                                Public methods                              //
// ************************************************************************** //

bool	checkCaracter(const std::string &line, const char &c)
{
	if (line.find(c) != std::string::npos)
		return (true);
	return (false);
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
			// if (line.empty() || (line.find("location") && checkCaracter(line, '{')) || line.find("server {") != std::string::npos)
			// 	continue;
			// else if (startParsing(line) == 1)
			// {
			// 	std::cerr << RED << "Error: Invalid config file" << RESET << std::endl;
			// 	break;
			// }
			// else
			// {
			// 	std::cerr << "Error: Invalid config file" << std::endl;
			// 	break;
			// }
			// std::cout << "Ci 6?" << std::endl;
			// else if (line.find("location") != std::string::npos)
			// {
				// std::cout << line << std::endl;
				// continue;
			// }
			        // Rimuovi spazi bianchi iniziali e finali
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
					inLocationBlock++;
        	        std::cout << "Entering location block" << std::endl;
        	        continue;
        	    }
				if (inLocationBlock != 0)
					parseLocationBlock(line);
        	    if (inLocationBlock) 
				{
        	        if (checkCaracter(line, '}')) 
					{
        	            inLocationBlock--;
        	            std::cout << "Exiting location block" << std::endl;
        	        }
        	        continue;
        	    }
        	    if (checkCaracter(line, '}')) 
				{
					if (inServerBlock == 1)
						inServerBlock--;
        	        std::cout << "Exiting server block" << std::endl;
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
	std::cout << "Parsing line in Server block: \t";
	std::string			content;
	std::string			token;
	std::istringstream	ss(line);
	
	// check se finisce con il char ';'
	// find end of line (position of ';'), start checking from left to right
	// std::cout << "This is line: " << line << std::endl;
	// if (line.find(''))
	size_t start = 0;
	while (line[start] && isspace(line[start]))
		start++;
	std::string key, value;
	std::string actual_start = line.substr(start);
	if (actual_start[0] == '#' or actual_start.size() == 0 or actual_start.empty())
		return (2);
	size_t end_key = actual_start.find(' ');
	if (end_key == std::string::npos)
	{
		//errore config non valido
		// std::cout << "This is end_key: " << actual_start << std::endl;
		// std::cerr << RED << "1) Error: Invalid config file" << RESET << std::endl;
		// stop parsing throw error ... 
	}
	else
	{
	// std::cout << "This is line: " << line << std::endl;
		key = actual_start.substr(0, end_key);
		// std::cout << "This is key: " << key << std::endl;
		size_t start_value = end_key;
		while (actual_start[start_value] and isspace(actual_start[start_value]))
			start_value++;
		// std::cout << actual_start.substr(start_value) << '\n';
		size_t n_chars = 0;
		while (actual_start[start_value + n_chars] and
		 	actual_start[start_value + n_chars] != ';')
			n_chars++;
		value = actual_start.substr(start_value, n_chars);
		std::cout << key << " - " << value << std::endl;
	}
	if (!checkCaracter(line, '}') && !checkCaracter(line, ';'))
		return (1);
	return (0);
}



int	Config::parseLocationBlock(const std::string &line)
{
	std::string			content;
	std::string			token;
	std::istringstream	ss(line);
	std::cout << "Parsing line in Location block: \t";
	
	// check se finisce con il char ';'
	// find end of line (position of ';'), start checking from left to right
	// std::cout << "This is line: " << line << std::endl;
	// if (line.find(''))
	size_t start = 0;
	while (line[start] && isspace(line[start]))
		start++;
	std::string key, value;
	std::string actual_start = line.substr(start);
	if (actual_start[0] == '#' or actual_start.size() == 0 or actual_start.empty())
		return (2);
	size_t end_key = actual_start.find(' ');
	if (end_key == std::string::npos)
	{
		//errore config non valido
		// std::cout << "This is end_key: " << actual_start << std::endl;
		// std::cerr << RED << "1) Error: Invalid config file" << RESET << std::endl;
		// stop parsing throw error ... 
	}
	else
	{
	// std::cout << "This is line: " << line << std::endl;
		key = actual_start.substr(0, end_key);
		// std::cout << "This is key: " << key << std::endl;
		size_t start_value = end_key;
		while (actual_start[start_value] and isspace(actual_start[start_value]))
			start_value++;
		// std::cout << actual_start.substr(start_value) << '\n';
		size_t n_chars = 0;
		while (actual_start[start_value + n_chars] and
		 	actual_start[start_value + n_chars] != ';')
			n_chars++;
		value = actual_start.substr(start_value, n_chars);
		std::cout << key << " - " << value << std::endl;
	}
	if (!checkCaracter(line, '}') && !checkCaracter(line, ';'))
		return (1);
	return (0);
}