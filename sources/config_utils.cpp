/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_utils.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/06 19:27:26 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/04/17 17:05:43 by mde-cloe      ########   odam.nl         */
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
	os << "Max Body Size: " << config._client_max_body_size << "\n";
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
	std::cout << "Total servers: " << configs.size() << std::endl;
	for (size_t i = 0; i < configs.size(); i++) {
		std::cout << "Server " << i + 1 << ":" << std::endl;
		std::cout << "Host: " << configs[i]._host << std::endl;
		std::cout << "Listen: " << configs[i]._listen << std::endl;
		std::cout << "Server Name: " << configs[i]._serverName << std::endl;
	}
	for (size_t i = 0; i < configs.size(); i++) {
		std::cout << LINE << std::endl;
		std::cout << "Server block:\n" << configs[i].toString() << std::endl;
		
		std::vector<std::string> &serverIndex = configs[i].getIndex();
		std::cout << "Server Index: ";
		for (const auto &idx : serverIndex) {
			std::cout << idx << ' ';
		}
		std::cout << std::endl;

		std::unordered_map<std::string, std::string> errorPage = configs[i].getErrorPage();

		if (errorPage.empty()) {
			std::cout << "Server ErrorPage: (empty)" << std::endl;
		} else {
			std::cout << "Server ErrorPage: ";
			for (const auto &idx : errorPage) {
				std::cout << "[" << idx.first << " -> " << idx.second << "] ";
			}
		}
		std::cout << std::endl;

		std::cout << "Total locations for server " << i << ": " << configs[i]._locations.size() << std::endl;
		std::cout << LINE << std::endl;

		for (size_t j = 0; j < configs[i]._locations.size(); j++) {
			std::cout << "Location block:\n" << configs[i]._locations[j].toString() << std::endl;

			std::vector<Http_method> locationAllowMethods = configs[i]._locations[j].getAllowMethods();
			std::cout << "Allow Methods: ";
			for (const auto &method : locationAllowMethods) {
				std::cout << (int)method << ' ';
			}
			std::cout << std::endl;

			std::vector<std::string> locationCgiExtensions = configs[i]._locations[j].getCgiExtension();
			std::cout << "CGI Extensions: ";
			for (const auto &extension : locationCgiExtensions) {
				std::cout << extension << ' ';
			}
			std::cout << std::endl;

			std::vector<std::string> locationCgiPaths = configs[i]._locations[j].getCgiPath();
			std::cout << "CGI Paths: ";
			for (const auto &path : locationCgiPaths) {
				std::cout << path << ' ';
			}
			std::cout << std::endl;

			std::vector<std::string> &locationIndex = configs[i]._locations[j].getIndex();
			std::cout << "Index: ";
			for (const auto &idx : locationIndex) {
				std::cout << idx << ' ';
			}
			std::cout << std::endl;

			std::cout << LINE << std::endl;

			for (size_t k = 0; k < configs[i]._locations[j]._nestedLocations.size(); k++) {
				std::cout << "NESTED location block:\n" 
						  << configs[i]._locations[j]._nestedLocations[k].toString() << std::endl;
				std::cout << LINE << std::endl;
			}
		}
	}
}
