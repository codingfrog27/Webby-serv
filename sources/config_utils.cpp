/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:27:26 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/11 14:13:23 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::ostream& operator<<(std::ostream& os, const Config& config)
{
	os << "Server Name: " << config._serverName << "\n";
	os << "Root: " << config._rootDir << "\n";
	os << "Listen: " << config._listen << "\n";
	os << "Host: " << config._host << "\n";
	os << "Error Page: " << config._errorPage << "\n";
	os << "Max Body Size: " << config._client_max_body_size << "\n";
	os << "Index: " << config._index << "\n";
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
