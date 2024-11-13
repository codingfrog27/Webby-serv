/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_error_checking.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: antoniosimone <antoniosimone@student.42      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/06 19:41:53 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/11/12 22:48:22 by antoniosimo   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <filesystem>

//change error_page to vector because it can have multiple values and we have to add them to the vector
//change index to vector because it can have multiple values and we have to add them to the vector

std::string	find_value(std::string& directive)
{
	std::string value;
	std::cout << directive << std::endl;
	
	if (directive.length() == 0)
		throw std::invalid_argument("Error: directive is empty");
	//if (directive.)
	int i = 0;
	while(directive[i] != ' ')
	{
		i++;
	} 
	//std::cout << value << std::endl;
	return (value);
}



// leggo la stringa la divido in due
//quando trovo lo spazio 
//la parola successiva è il valore.
//il valore termina quando finisce la stringa.

// To be continued...
std::string Config::validateErrorPage()
{
	std::string errorPage;
	if (!_rulemap.contains("error_page"))
		throw std::invalid_argument("Error: error_page directive not found");
	
	errorPage = normalize_space(_rulemap.at("error_page"));
	// std::cout << errorPage << std::endl;
	// normalize_space(errorPage);
	// std::cout << errorPage << std::endl;
	// for (auto i : _rulemap.at("error_page"))
	// {
	// 	errorPage.insert(_rulemap.)
	// }
	
	// for (auto i : _rulemap)
	// 	for(auto x : i.second)
	// 	{
	// 		if (_rulemap)
	// 		std::cout << i.first << i.second << std::endl;
	// 	}

	// std::cout << "I'm here!" << std::endl;
	// print_map(_errorPage);
	
	return (errorPage);
}
// To be continued...
std::string Config::validateIndex()
{
	std::string index_rule;
	std::string index_value;
	
	if (!_rulemap.contains("index"))
		throw std::invalid_argument("Error: index directive not found");
	index_rule = _rulemap.at("index");
	index_value = find_value(index_rule);
	//std::cout << index_rule << std::endl;

	//std::string temp;


	// std:: cout << "index: " << index << std::endl;
	return (index_rule);
}

std::string	Config::validateListen()
{
	std::string listen_rule;;
	std::string listen_value;
	
	if (!_rulemap.contains("listen"))
		throw std::invalid_argument("Error: listen directive not found");
		
	listen_rule = _rulemap.at("listen");
	listen_value = find_value(listen_rule);
	
	for (auto i = 0; i < listen_value.length(); i++)
	{
		if (!isdigit(listen_value[i]))// || listen_value.empty())
			throw std::invalid_argument("Error: invalid character in listen directive");
	}
	
	return (listen_value);
}

std::string Config::validateMaxBodySize()
{
	std::string maxBodySize_rule;
	std::string maxBodySize_value;
	
	if (!_rulemap.contains("client_max_body_size"))
		throw std::invalid_argument("Error: client_max_body_size directive not found");
		
	maxBodySize_rule = _rulemap.at("client_max_body_size");
	maxBodySize_value = find_value(maxBodySize_rule);
	
	char lastChar = maxBodySize_value.back();
	if (lastChar == 'k' || lastChar == 'K' || lastChar == 'm' || lastChar == 'M' || lastChar == 'g' || lastChar == 'G')
		maxBodySize_value.pop_back();
	else
		throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	
	for(auto i = 0; i < maxBodySize_value.size(); i++)
	{
		if (!isdigit(maxBodySize_value[i]))// || maxBodySize.empty())
			throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	}
	return (maxBodySize_value + lastChar);
}

std::string Config::validateHost()
{
	std::string host_rule;
	std::string host_value;
	
	if (!_rulemap.contains("host"))
		throw std::invalid_argument("Error: host directive not found");
		
	host_rule = _rulemap.at("host");
	host_value = find_value(host_rule); 

	int dotCount = 0;
	for(auto i = 0; i < host_value.length(); i++)
	{
		if (host_value[i] == '.')
		{
			dotCount++;
			i++;
		}
		if (!isdigit(host_value[i]) && host_value[i] != '.')
			throw std::invalid_argument("Error: invalid character in host directive");
	}
	if (dotCount != 3)
		throw std::invalid_argument("Error: invalid host directive");
	return (host_rule);
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