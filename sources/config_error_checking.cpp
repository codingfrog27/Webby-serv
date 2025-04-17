/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_error_checking.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/06 19:41:53 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/04/17 14:16:46 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <filesystem>
#include <limits>

std::string find_value(std::string &directive)
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
	if (value.length() == 0)
		throw std::invalid_argument("Error: directive is empty");
	return (value);
}

std::string getErrorPageMapKey(std::string &errorPage_value)
{
	int digits = 0;
	std::string errorPage_key;

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
	}
	return (errorPage_key);
}

std::unordered_map<std::string, std::string> Config::validateErrorPage()
{
	std::unordered_map<std::string, std::string> tmpErrorPageMap;

	if (_rulemap.contains("error_page"))
	{
		auto found = _rulemap.find("error_page");
		std::string errorPage_rule = normalize_space(found->second);
		std::istringstream iss(errorPage_rule);
		std::string token, errorPage;
		std::vector<std::string> errorCodes;

		iss >> token;

		while (iss >> token)
		{
			if (std::isdigit(token[0]))
				errorCodes.push_back(token);
			else
			{
				errorPage = token;
				for (char c : errorPage)
				{
					if (!isdigit(c) && !isalpha(c) && c != '/' && c != '.' && c != '_')
						throw std::invalid_argument("Error: invalid character in error_page directive");
				}
				for (const auto &code : errorCodes)
					tmpErrorPageMap[code] = errorPage;
				errorCodes.clear();
			}
		}
	}
	return (tmpErrorPageMap);
}

std::vector<std::string> Config::ValidateIndex()
{
	std::vector<std::string> tmp_vector;
	std::string index_rule, index_value;

	if (_rulemap.contains("index"))
	{
		index_rule = normalize_space(_rulemap.at("index"));
		index_value = find_value(index_rule);
	}
	else
		return (tmp_vector);

	for (char c : index_value)
	{
		if (!std::isalnum(c) && c != '-' && c != '.' && !std::isspace(c))
		{
			throw std::invalid_argument("Error: invalid character in index directive");
		}
	}

	std::istringstream iss(index_value);
	std::string word;
	while (iss >> word)
		tmp_vector.push_back(word);

	return tmp_vector;
}

std::string Config::validateListen()
{
	std::string listen_rule;
	;
	std::string listen_value;

	if (_rulemap.contains("listen"))
	{
		auto found = _rulemap.find("listen");
		listen_rule = normalize_space(found->second);
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
	if (lastChar == 'k' || lastChar == 'K' || lastChar == 'm' || lastChar == 'M' || lastChar == 'g' || lastChar == 'G' || std::isdigit(lastChar))
		maxBodySize_value.pop_back();
	else
		throw std::invalid_argument("Error: invalid character in client_max_body_size directive");

	for (size_t i = 0; i < maxBodySize_value.size(); i++)
	{
		if (!isdigit(maxBodySize_value[i]))
			throw std::invalid_argument("Error: invalid character in client_max_body_size directive");
	}

	return (maxBodySize_value + lastChar);
}

size_t Config::convertMaxBodySize()
{
	char lastChar = _client_max_body_size.back();

	std::string maxBodySize_value;
	if (std::isalpha(lastChar))
		maxBodySize_value = _client_max_body_size.substr(0, _client_max_body_size.length() - 1);
	else
		maxBodySize_value = _client_max_body_size;

	unsigned long long result = 0;
	try
	{
		result = std::stoull(maxBodySize_value);
		if (lastChar == 'k' || lastChar == 'K')
			result *= 1024;
		else if (lastChar == 'm' || lastChar == 'M')
			result *= 1024 * 1024;
		else if (lastChar == 'g' || lastChar == 'G')
			result *= 1024 * 1024 * 1024;
	}
	catch (const std::out_of_range &e)
	{
		throw std::invalid_argument("Error: client_max_body_size value is too large");
	}
	catch (const std::invalid_argument &e)
	{
		throw std::invalid_argument("Error: invalid client_max_body_size value");
	}

	if (result > std::numeric_limits<size_t>::max())
		throw std::invalid_argument("Error: client_max_body_size value is too large");

	return result;
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

	std::vector<std::string> octets;
	std::stringstream ss(host_value);
	std::string segment;
	int dotCount = 0;

	while (std::getline(ss, segment, '.'))
	{
		octets.push_back(segment);
		dotCount++;
	}

	if (dotCount != 4)
	{
		throw std::invalid_argument("Error: invalid host directive (incorrect number of octets)");
	}

	for (const std::string &octet : octets)
	{
		if (octet.empty() || octet.length() > 3)
			throw std::invalid_argument("Error: invalid octet in host directive");
		for (char c : octet)
		{
			if (!isdigit(c))
				throw std::invalid_argument("Error: invalid character in host directive");
		}

		int num = std::stoi(octet);
		if (num < 0 || num > 255)
			throw std::invalid_argument("Error: invalid IP range in host directive");
	}
	return host_value;
}

size_t Config::validateTimeout()
{
	std::string timeout_rule;
	std::string timeout_value;
	size_t timeout_size_t;

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
	for (size_t i = 0; i < serverName_value.length(); i++)
	{
		if (!isalpha(serverName_value[i]) && !isdigit(serverName_value[i]) &&
			serverName_value[i] != '-' && serverName_value[i] != '.' && serverName_value[i] != '_')
			throw std::invalid_argument("Error: invalid character in server_name directive");
	}

	return (serverName_value);
}

std::string Config::validateRoot()
{
	std::string root_rule;
	std::string root_value;
	size_t dot = 0;

	if (_rulemap.contains("root"))
	{
		auto found = _rulemap.find("root");
		root_rule = normalize_space(found->second);
		root_value = find_value(root_rule);
	}
	else 
		throw std::invalid_argument("Error: root directive not found");

	size_t root_value_lenght = root_value.length();
	if (root_value_lenght == 1)
		throw std::invalid_argument("Error: invalid root path in root directive");

	for (size_t i = 0; i < root_value_lenght; i++)
	{
		if (root_value[0] == '.')
			dot++;
		else if (root_value[0] != '/' || root_value[root_value_lenght - 1] == '/')
			throw std::invalid_argument("Error: invalid root path directive:" \
			"please start root with '/' "  + root_value + " or it doesn't have to finish wiht a /");

		if (!isalpha(root_value[i]) && !isdigit(root_value[i]) && root_value[i] != '/' && root_value[i] != '_' && root_value[i] != '-' && dot > 1)
			throw std::invalid_argument("Error: invalid character in root directive");
	}
	std::filesystem::path rootPath(root_value);

	if (!rootPath.is_absolute())
	    rootPath = std::filesystem::current_path() / rootPath;
	if (!std::filesystem::exists(rootPath))
	    throw std::invalid_argument("Error: root path does not exist! (" + rootPath.string() + ")");
	if (!std::filesystem::is_directory(rootPath))
	    throw std::invalid_argument("Error: root path is not a directory! (" + rootPath.string() + ")");

	if (root_value[0] == '/')
		root_value.erase(0 , 1);
	else if (root_value[0] == '.' && root_value[1] == '/')
		root_value.erase(0 , 2);
	return (root_value);
}

bool Config::validateAutoindex()
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

void checkPortUniqueness(const std::vector<std::unique_ptr<Config>> &configs)
{
	for (size_t i = 0; i < configs.size(); i++)
	{
		const std::string &port = configs[i]->getListen();
		for (size_t j = i + 1; j < configs.size(); j++)
		{
			if (port == configs[j]->getListen())
				throw(std::invalid_argument("Multiple server blocks listening to the same port!"));
		}
	}
}
