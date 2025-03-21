/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:27:26 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/02/03 20:05:59 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "NicePrint.hpp"

std::ostream& operator<<(std::ostream& os, const Config& config)
{
	os << "Server Name: " << config._serverName << std::endl;
	os << "Root: " << config._rootDir << std::endl;
	os << "Listen: " << config._listen << std::endl;
	os << "Host: " << config._host << std::endl;
	os << "Max Body Size: " << config._client_max_body_size << std::endl;
	os << "Server Index: ";
	for (const std::string &idx : config._index) {
		std::cout << idx << ' ';
	}
	os << "Server ErrorPage: ";
	for (const auto &idx : config._errorPage) {
		os << idx.first << " " << idx.second;
	}
	// os << "Error Page: " << config._errorPage << std::endl;
	// os << "Index: " << config._index << std::endl;
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

void printConfigs(std::vector<Config> &configs)
{
	for (size_t i = 0; i < configs.size(); i++) {
		std::cout << YELLOW LINE "\nServer block: " << i + 1 << configs[i] << RESET << std::endl;

		for (size_t j = 0; j < configs[i]._locations.size(); j++) {
			std::cout << MAGENTA LINE_S "Location block:\n" << configs[i]._locations[j];
			std::cout <<  LINE_S RESET << std::endl;
			for (size_t k = 0; k < configs[i]._locations[j]._nestedLocations.size(); k++) {
				std::cout << CYAN LINE_S "NESTED location block: " << configs[i]._locations[j]._nestedLocations[k] << std::endl;
				std::cout <<  LINE RESET << std::endl;
			}
		}
		std::cout << YELLOW LINE << std::endl;
	}
}
