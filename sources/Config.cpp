/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:10:04 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/08 16:18:33 by asimone          ###   ########.fr       */
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


void Config::parseConfigFile(const std::string fileName)
{
	std::string			line;
	std::ifstream		file(fileName);
	

	if (!file.is_open())
	{
		std::cerr << "Error: Unable to open file" << std::endl;
		return ;
	}
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (line.find("server {") != std::string::npos)
			{
				// std::cout << "START SERVER PARSER" << std::endl;
				continue;
			}
			// else
			// {
			// 	std::cerr << "Error: Invalid config file" << std::endl;
			// 	break;
			// }
			// std::cout << "Ci 6?" << std::endl;
			startParsing(line);
		}
		file.close();
	}
	else 
	{
		std::cerr << "Error: Unable to open file" << std::endl;
	}
	return ;
}

void	Config::startParsing(const std::string &line)
{
	std::string			content;
	std::string			token;
	std::istringstream	ss(line);
	
	// check se finisce con il char ';'
	// find end of line (position of ';'), start checking from left to right
	size_t start = 0;
	while (line[start] && isspace(line[start]))
		start++;
	std::string key, value;
	std::string actual_start = line.substr(start);
	if (actual_start[0] == '#' or actual_start.size() == 0 or actual_start.empty())
		return;
	size_t end_key = actual_start.find(' ');
	if (end_key == std::string::npos)
	{
		//errore config non valido
		std::cout << "error\n";
		// stop parsing throw error ... 
	}
	else
	{
		key = actual_start.substr(0, end_key);
		size_t start_value = end_key;
		while (actual_start[start_value] and isspace(actual_start[start_value]))
			start_value++;
		// std::cout << actual_start.substr(start_value) << '\n';
		size_t n_chars = 0;
		while (actual_start[start_value + n_chars] and
		 	!isspace(actual_start[start_value + n_chars]) and
		 	actual_start[start_value + n_chars] != ';')
			n_chars++;
		value = actual_start.substr(start_value, n_chars);
		std::cout << key << " - " << value << std::endl;
	}

	
		// }
		// if (line.find("host") != std::string::npos)
		// {
		// 	while (std::getline(ss, token, ';'))
		// 	{
		// 		std::cout << "This is host: " << token << std::endl;
		// 		break;
		// 	}	
		// }
	// }

		
	// std::string::const_iterator it = line.begin();
	// size_t i = 0;

	// std::cout << line << std::endl;
	
	// while (it != line.end())
	// {
	// 	std::cout << "i= " << i;
		// std::cout << *it << std::endl;
		// if(*it == ' ')
		// {
		// 	it++;
		//  	continue;
		// }
		// if(line.find("server"))
		// {
		// std::cout << line << std::endl;
			// break;
		//}
		// if (line.find("listen"))
		// {
		// 	std::cout << "listen" << std::endl;
		// 	// break;
		// }
		// i++;
		// ++it;
	// }
}