/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_error_checking.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:41:53 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/18 16:56:09 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <filesystem>

std::string	find_value(std::string& directive)
{
	std::string value;
	
	if (directive.length() == 0)
		throw std::invalid_argument("Error: directive is empty");
		
	int i = 0;
	
	while (directive[i])
	{
		if (isspace(directive[i]))
		{
			value = directive.substr(i + 1, i - directive.length());
			break;
		}
		i++;
	}
	return (value);
}

std::string	getErrorPageMapKey(std::string& errorPage_value)
{
	int digits = 0;
	std::string	errorPage_key;

	for (auto i = 0; i < errorPage_value[i]; i++)
	{
		if (isspace(errorPage_value[i]))
			errorPage_key = errorPage_value.substr(0, i);
	}
	for (auto i = 0; i < errorPage_key[i]; i++)
	{
		if (isdigit(errorPage_key[i]))
			digits++;
		// if (digits > 3 || !isdigit(errorPage_key[i]))
		// 	throw std::invalid_argument("Error: invalid error_page code directive");
	}
	return (errorPage_key);
}

std::string getErrorPageMapValue(std::string& errorPage_value)
{
	std::string errorPage_path = errorPage_value.substr(4, errorPage_value.length());

	for (auto i = 0; i < errorPage_path.length(); i++)
	{
		// if (!isdigit(errorPage_path[i]) && !isalpha(errorPage_path[i]) && errorPage_path[i] != '/' && errorPage_path[i] != '.')
		// 	throw std::invalid_argument("Error: invalid character in error_page directive");
	}
	return (errorPage_path);
}

std::unordered_map<std::string, std::string>		Config::validateErrorPage()
{
	std::string errorPage_rule;
	std::string errorPage_value;
	std::unordered_map<std::string, std::string> tmpErrorPageMap;
	size_t find_space = 0;
	int space = 0;
	
	if (!_rulemap.contains("error_page"))
		throw std::invalid_argument("Error: error_page directive not found");
	
	errorPage_rule = normalize_space(_rulemap.at("error_page"));
	errorPage_value = find_value(errorPage_rule);

	for (auto i = 0; i < errorPage_value.length(); i++)
	{
		if (isspace(errorPage_value[i]))
		{
			find_space = errorPage_value.find(' ');
			if (errorPage_value[find_space + 1] != '/')
				throw std::invalid_argument("Error: invalid path in error_page directive");
			space++;
			i++;
		}
		// if (space > 1)
		// 	throw std::invalid_argument("Error: invalid character in error_page directive");
	}
	
	std::string map_key = getErrorPageMapKey(errorPage_value);
	std::string map_value = getErrorPageMapValue(errorPage_value);
	tmpErrorPageMap.emplace(map_key, map_value);
	return (tmpErrorPageMap);
}

std::vector<std::string>		Config::ValidateIndex()
{
	std::string index_rule;
	std::string index_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;
	
	int space_pos = 0;
	static int space = 0;
	
	if (!_rulemap.contains("index"))
		throw std::invalid_argument("Error: index directive not found");

	index_rule = normalize_space(_rulemap.at("index"));
	index_value = find_value(index_rule);
	
	for (auto i = 0; i < index_value.length(); i++)
	{
		if (isspace(index_value[i]))
			i++;
		if (!isalpha(index_value[i]) && !isdigit(index_value[i]) && index_value[i] != '-' && index_value[i] != '.')
			throw std::invalid_argument("Error: invalid character in index directive");
	}
	for (auto i = 0; i < index_value.length(); i++)
	{
		if (isspace(index_value[i]))
			space++;
	}
	if (space == 0)
	{
		tmp_value = index_value.substr(0, index_value.length());
		tmp_vector.push_back(tmp_value);
	}
	else
	{
		int j = 0;
		for (int i = 0; i < index_value.length(); i++)
		{
			if (!isspace(index_value[i]))
			{	
				for (j = i; !isspace(index_value[j]) && j < (index_value.length()); j++)
					tmp_value = index_value.substr(i, j - i);
				tmp_vector.push_back(tmp_value);
				i = j;	
			}
		}
	}
	for (auto i = tmp_vector.begin(); i < tmp_vector.end(); i++)
		std::cout << *i << std::endl;
	return (tmp_vector);
}

std::string	Config::validateListen()
{
	std::string listen_rule;;
	std::string listen_value;
	
	if (!_rulemap.contains("listen"))
		throw std::invalid_argument("Error: listen directive not found");
		
	listen_rule = normalize_space (_rulemap.at("listen"));
	listen_value = find_value(listen_rule);

	for (auto i = 0; i < listen_value.length(); i++)
	{
		if (!isdigit(listen_value[i]))
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
		
	maxBodySize_rule = normalize_space(_rulemap.at("client_max_body_size"));
	maxBodySize_value = find_value(maxBodySize_rule);
	
	char lastChar = maxBodySize_value.back();
	if (lastChar == 'k' || lastChar == 'K' || lastChar == 'm' || lastChar == 'M' || lastChar == 'g' || lastChar == 'G')
		maxBodySize_value.pop_back();
	else
		throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	
	for(auto i = 0; i < maxBodySize_value.size(); i++)
	{
		if (!isdigit(maxBodySize_value[i]))
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
		
	host_rule = normalize_space(_rulemap.at("host"));
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
	std::string serverName_rule;
	std::string serverName_value;
	
	
	if (!_rulemap.contains("server_name"))
		throw std::invalid_argument("Error: server_name directive not found");
	serverName_rule = normalize_space(_rulemap.at("server_name"));
	serverName_value = find_value(serverName_rule);
	
	for(auto i = 0; i < serverName_value.length(); i++)
	{
		if (!isalpha(serverName_value[i]) && !isdigit(serverName_value[i]) && serverName_value[i] != '-' && serverName_value[i] != '.')
			throw std::invalid_argument("Error: invalid character in server_name directive");
	}

	return (serverName_value);
}

std::string Config::validateRoot()
{
	std::string root_rule;
	std::string root_value;

	if (!_rulemap.contains("root"))
		throw std::invalid_argument("Error: root directive not found");
	
	root_rule = normalize_space(_rulemap.at("root"));
	root_value = find_value(root_rule);

	for (auto i = 0; i < root_value.length(); i++)
	{
		if (root_value[0] != '/')
			throw std::invalid_argument("Error: invalid root path directive");
		if (!isdigit(root_value[i]) && !isalpha(root_value[i]) && root_value[i] != '/' && root_value[i] != '.' && root_value[i] != '_')
			throw std::invalid_argument("Error: invalid root path directive");
	}
	
	return (root_value);
}