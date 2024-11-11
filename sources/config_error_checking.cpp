/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_error_checking.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:41:53 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/11 16:43:57 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <filesystem>

//change error_page to vector because it can have multiple values and we have to add them to the vector
//change index to vector because it can have multiple values and we have to add them to the vector

// To be continued...
std::string Config::validateErrorPage()
{
	std::string errorPage;
	if (!_rulemap.contains("error_page"))
		throw std::invalid_argument("Error: error_page directive not found");
	errorPage = _rulemap.at("error_page");
	
	return (errorPage);
}
// To be continued...
std::string Config::validateIndex()
{
	std::string index;
	if (!_rulemap.contains("index"))
		throw std::invalid_argument("Error: index directive not found");
	index = _rulemap.at("index");

	std::string temp;


	// std:: cout << "index: " << index << std::endl;
	return (index);
}

std::string	Config::validateListen()
{
	std::string listen;
	if (!_rulemap.contains("listen"))
		throw std::invalid_argument("Error: listen directive not found");
	listen = _rulemap.at("listen");
	for(auto i = 0; i < listen.size(); i++)
	{
		if (!isdigit(listen[i]) || listen.empty())
			throw std::invalid_argument("Error: invalid character in listen directive");
	}
	return (listen);
}

std::string Config::validateMaxBodySize()
{
	std::string maxBodySize;
	if (!_rulemap.contains("client_max_body_size"))
		throw std::invalid_argument("Error: client_max_body_size directive not found");
	maxBodySize = _rulemap.at("client_max_body_size");

	char lastChar = maxBodySize.back();
	if (lastChar == 'k' || lastChar == 'K' || lastChar == 'm' || lastChar == 'M' || lastChar == 'g' || lastChar == 'G')
		maxBodySize.pop_back();
	else
		throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	
	for(auto i = 0; i < maxBodySize.size(); i++)
	{
		if (!isdigit(maxBodySize[i]) || maxBodySize.empty())
			throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	}
	return (maxBodySize + lastChar);
}

std::string Config::validateHost()
{
	std::string host;
	if (!_rulemap.contains("host"))
		throw std::invalid_argument("Error: host directive not found");
	host = _rulemap.at("host");

	int dotCount = 0;

	for(auto i = 0; i < host.size(); i++)
	{
		if (host[i] == '.')
		{
			dotCount++;
			i++;
		}
		if (!isdigit(host[i]) || host.empty())
			throw std::invalid_argument("Error: invalid character in host directive");
	}
	if (dotCount != 3)
		throw std::invalid_argument("Error: invalid host directive");
	return (host);
}

std::string Config::validateServerName()
{
	std::string serverName;
	if (!_rulemap.contains("server_name"))
		throw std::invalid_argument("Error: server_name directive not found");
	serverName = _rulemap.at("server_name");

	return (serverName);
}

std::string Config::validateRoot()
{
	std::string root;
	if (!_rulemap.contains("root"))
		throw std::invalid_argument("Error: root directive not found");
	
	root = _rulemap.at("root");
	// std::filesystem::path& root;
	// std::filesystem::status(root).permissions();
	// std::filesystem::perms p;
    // using std::filesystem::perms;
    // auto show = [=](char op, perms perm)
    // {
    //     std::cout << (perms::none == (perm & p) ? '-' : op);
    // };
	// show('r', perms::owner_read);
    // show('w', perms::owner_write);
    // show('x', perms::owner_exec);
	// std::cout << '\n';
	// std::cout << "__________________" << '\n';
    // show('r', perms::group_read);
    // show('w', perms::group_write);
    // show('x', perms::group_exec);
	// std::cout << '\n';
	// std::cout << "__________________" << '\n';;
    // show('r', perms::others_read);
    // show('w', perms::others_write);
    // show('x', perms::others_exec);
    // std::cout << '\n';

	
	
	return (root);
}