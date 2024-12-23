/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:27:26 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/23 15:43:54 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "NicePrint.hpp"

std::ostream& operator<<(std::ostream& os, const Config& config)
{
	os << "Server Name: " << config._serverName << "\n";
	os << "Root: " << config._rootDir << "\n";
	os << "Listen: " << config._listen << "\n";
	os << "Host: " << config._host << "\n";
	// os << "Error Page: " << config._errorPage << "\n";
	os << "Max Body Size: " << config._client_max_body_size << "\n";
	// os << "Index: " << config._index << "\n";
    return os;
}


// ************************************************************************** //
//								GETTER & SETTER								  //
// ************************************************************************** //


bool	checkCaracter(const std::string &line, const char &c)
{
	if (line.find(c) != std::string::npos)
		return (true);
	return (false);
}

bool	checkstr(const std::string &line, const std::string &str)
{
	if (line.find(str) != std::string::npos)
		return (true);
	return (false);
}

bool	locationFound(std::string &line)
{
	if (checkstr(line, "location") && checkCaracter(line, '{'))
		return (true);
	return (false);
}

void	print_map(const std::unordered_map<std::string, std::string> map)
{
	for(auto i : map)
	{
		// for (auto x : i.second)
		std::cout << "Key: " << i.first << "\t" << "Value: " << i.second << std::endl;
	}
}

std::string	normalize_space(std::string& str)
{
	int is_space = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isspace(str[i]))
			is_space++;
		else
		{
			if (is_space > 1)
			{
				str.erase(i - is_space, is_space - 1);
				i -= (is_space -1);
			}
			is_space = 0;
		}
	}
	return (str);
}

void	printConfigs(const std::vector<Config> &configs)
{
	for (size_t i = 0; i < configs.size(); i++){
		std::cout << "server block:\n" << configs[i].toString() << LINE << std::endl;
		for (size_t j = 0; j < configs[i]._newLocations.size(); j++){
			std::cout << "location block:\n" << configs[i]._newLocations[j]->toString() \
			<< "---------------" << std::endl;
			for (size_t k = 0; k < configs[i]._newLocations[j]->_nestedLocations.size(); k++)
			{
				std::cout << "NESTED location block:\n" << configs[i]._newLocations[j]->_nestedLocations[0]->toString()\
					<< std::endl;
			}
		}
	}
}
