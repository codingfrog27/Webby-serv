/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_error_checking.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:41:53 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/19 15:09:38 by mde-cloe         ###   ########.fr       */
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

	(void)digits;
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

	for (size_t i = 0; i < errorPage_path.length(); i++)
	{
		// if (!isdigit(errorPage_path[i]) && !isalpha(errorPage_path[i]) && errorPage_path[i] != '/' && errorPage_path[i] != '.')
		// 	throw std::invalid_argument("Error: invalid character in error_page directive");
	}
	return (errorPage_path);
}

std::multimap<std::string, std::string> Config::validateErrorPage()
{
    std::multimap<std::string, std::string> tmpErrorPageMap;

    auto range = _rulemap.equal_range("error_page");
    for (auto it = range.first; it != range.second; ++it)
    {
        std::string errorPage_rule = normalize_space(it->second);
        std::istringstream iss(errorPage_rule);
        std::string errorCode;
        std::string errorPage;

        while (iss >> errorCode >> errorPage)
        {
            // if (errorPage[0] != '/')
            //     throw std::invalid_argument("Error: invalid path in error_page directive");

            for (char c : errorPage)
            {
                if (!isdigit(c) && !isalpha(c) && c != '/' && c != '.' && c != '_')
                    throw std::invalid_argument("Error: invalid character in error_page directive");
            }
            tmpErrorPageMap.emplace(errorCode, errorPage);
        }
    }

    if (tmpErrorPageMap.empty())
        throw std::invalid_argument("Error: no valid error_page directives found");

    return tmpErrorPageMap;
}

std::vector<std::string>		Config::ValidateIndex()
{
	std::string index_rule;
	std::string index_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;

	static int space = 0;

	if (_rulemap.contains("index"))
	{
		auto found = _rulemap.find("index");
		index_rule = normalize_space(found->second);
		index_value = find_value(index_rule);
	}
	else
		throw std::invalid_argument("Error: index directive not found");
	for (size_t i = 0; i < index_value.length(); i++)
	{
		if (isspace(index_value[i]))
			i++;
		if (!isalpha(index_value[i]) && !isdigit(index_value[i]) && index_value[i] != '-' && index_value[i] != '.')
			throw std::invalid_argument("Error: invalid character in index directive");
	}
	for (size_t i = 0; i < index_value.length(); i++)
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
		size_t j = 0;
		for (size_t i = 0; i < index_value.length(); i++)
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
	return (tmp_vector);
}

std::string	Config::validateListen()
{
	std::string listen_rule;;
	std::string listen_value;

	if (_rulemap.contains("listen"))
	{
		auto found = _rulemap.find("listen");
		listen_rule = normalize_space (found->second);
		listen_value = find_value(listen_rule);
	}
	else
		throw std::invalid_argument("Error: listen directive not found");
	for (size_t i = 0; i < listen_value.length(); i++)
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

	if (_rulemap.contains("client_max_body_size"))
	{
		auto found = _rulemap.find("client_max_body_size");
		maxBodySize_rule = normalize_space(found->second);
		maxBodySize_value = find_value(maxBodySize_rule);
	}
	else
		throw std::invalid_argument("Error: client_max_body_size directive not found");
	char lastChar = maxBodySize_value.back();
	if (lastChar == 'k' || lastChar == 'K' || lastChar == 'm' || lastChar == 'M' || lastChar == 'g' || lastChar == 'G')
		maxBodySize_value.pop_back();
	else
		throw std::invalid_argument("Error: invalid character in client_max_body_size directive");

	for(size_t i = 0; i < maxBodySize_value.size(); i++)
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

	if (_rulemap.contains("host"))
	{
		host_rule = normalize_space(_rulemap.at("host"));
		host_value = find_value(host_rule);
	}
	else
		throw std::invalid_argument("Error: host directive not found");
	int dotCount = 0;
	for(size_t i = 0; i < host_value.length(); i++)
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

size_t Config::validateTimeout()
{
	std::string timeout_rule;
	std::string timeout_value;
	size_t		timeout_size_t;

	if (_rulemap.contains("timeout"))
	{
		auto found = _rulemap.find("timeout");
		timeout_rule = normalize_space(found->second);
		timeout_value = find_value(timeout_rule);
	}
	else 
		return (std::stoi("3000"));

	timeout_size_t = stoi(timeout_value);
	
	if (timeout_size_t > MAX_TIMEOUT || timeout_size_t < 3000)
		throw std::invalid_argument("Error: invalid parameter in timeout directive");
	
	return (timeout_size_t);
}

std::string Config::validateServerName()
{
	std::string serverName_rule;
	std::string serverName_value;
	
	if (_rulemap.contains("server_name"))
	{
		auto found = _rulemap.find("server_name");
		serverName_rule = normalize_space(found->second);
		serverName_value = find_value(serverName_rule);
	}
	else
		return ("Default name");
	for(size_t i = 0; i < serverName_value.length(); i++)
	{
		if (!isalpha(serverName_value[i]) && !isdigit(serverName_value[i]) && \
			serverName_value[i] != '-' && serverName_value[i] != '.' && serverName_value[i] != '_')
			throw std::invalid_argument("Error: invalid character in server_name directive");
	}

	return (serverName_value);
}

std::string Config::validateRoot()
{
	std::string root_rule;
	std::string root_value;

	if (_rulemap.contains("root"))
	{
		auto found = _rulemap.find("root");
		root_rule = normalize_space(found->second);
		root_value = find_value(root_rule);
	}
	else
		throw std::invalid_argument("Error: root directive not found");
	for (size_t i = 0; i < root_value.length(); i++)
	{
		if (root_value[0] != '/')
			throw std::invalid_argument("Error: invalid root path directive:" \
			"please start root with /"  + root_value);
		if (!isdigit(root_value[i]) && !isalpha(root_value[i]) && root_value[i] \
			!= '/' && root_value[i] != '.' && root_value[i] != '_')
			throw std::invalid_argument("Error: invalid root path directive");
	}
	root_value.erase(0, 1); //hi antonio im removing leading root / for correct filepathing :)
	return (root_value);
}

bool		Config::validateAutoindex()
{
	std::string _autoIndex;
	std::string _autoIndex_value;

	if (_rulemap.contains("autoindex"))
	{
		auto found = _rulemap.find("autoindex");
		_autoIndex = normalize_space(found->second);
		_autoIndex_value = find_value(_autoIndex);
	}
	else
		return (true);

	if (_autoIndex_value.compare("on") == 0)
		return (true);
	else if (_autoIndex_value.compare("off") == 0)
		return (false);
	else 
		throw std::invalid_argument("Error: invalid character in autoindex directive");
	
	return (true);
}
/**
 * @brief A simple check to see if none of the server blocks are trying to listen on the same port
 *
 * @param configs vector of config objects to check
 */
void	checkPortUniqueness(const std::vector<std::unique_ptr<Config>> &configs)
{
	for (size_t i = 0; i < configs.size(); i++)
	{
		const std::string &port = configs[i]->getListen();
		for (size_t j = i + 1; j < configs.size(); j++)
		{
			if (port == configs[j]->getListen())
				throw (std::invalid_argument("Multiple server blocks listening to the same port!"));
		}
	}
}
