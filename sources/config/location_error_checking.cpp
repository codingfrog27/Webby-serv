#include "location.hpp"
#include <sstream>
#include <filesystem>
#include <string.h>

std::string	find_value_location(std::string& directive)
{
	std::string value;
	
	if (directive.length() == 0)
		throw std::invalid_argument(" directive is empty");
		
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
		throw std::invalid_argument(" directive is empty");
	return (value);
}

std::string	normalize_space_location(std::string& str)
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

std::vector<Http_method> location::validateAllowMethods()
{
	std::string allow_methods_rule;
    std::string allow_methods_value;
	std::string tmp_value;
	std::vector<Http_method>  tmp_vector;
	std::vector<std::string>  methods = {"GET", "POST", "DELETE"};
	
	static int space = 0;

    if (_rulemap.contains("allow_methods"))
	{
    	allow_methods_rule = normalize_space_location(_rulemap.at("allow_methods"));
		allow_methods_value = find_value_location(allow_methods_rule);
	}
	else 
		return (tmp_vector);
	for (size_t i = 0; i < allow_methods_value.length(); i++)
	{
		if (isspace(allow_methods_value[i]))
			i++;
		if (!isalpha(allow_methods_value[i]))
			throw std::invalid_argument(" invalid character in allow_methods directive");
	}
	for (size_t i = 0; i < allow_methods_value.length(); i++)
	{
		if (isspace(allow_methods_value[i]))
			space++;
	}
	if (space == 0)
	{
		tmp_value = allow_methods_value.substr(0, allow_methods_value.length());
		for (size_t i = 0; i < methods.size(); i++)
		{
			if (tmp_value == methods[i])
				tmp_vector.push_back((Http_method)i);
		}
		if (tmp_vector.empty())
		 	throw std::invalid_argument(" invalid ruleeeee in allow_methods directive");
	}
	else
	{
		for (size_t i = 0; i < allow_methods_value.length();)
		{
			while (i < allow_methods_value.length() && isspace(allow_methods_value[i]))
				i++;

			size_t start = i;
			while (i < allow_methods_value.length() && !isspace(allow_methods_value[i]))
				i++;

			if (start < i)
			{
				tmp_value = allow_methods_value.substr(start, i - start);
				bool valid = false;
				for (size_t i = 0; i < methods.size(); i++)
				{
					if (tmp_value == methods[i])
					{
						tmp_vector.push_back((Http_method)i);
						valid = true;
						break;
					}
				}
				if (tmp_vector.empty())
					throw std::invalid_argument(" invalid rule in allow_methods directive");
				else if (!valid)
				{
					std::cerr << "Invalid method: " << tmp_value << std::endl;
					throw std::invalid_argument(" invalid rule in allow_methods directive");
				}
			}
		}
	}
	return (tmp_vector);
}

bool location::validateAutoindex()
{
	std::string autoindex_rule;
	std::string autoindex_value;

	if (_rulemap.contains("autoindex"))
	{
		autoindex_rule = normalize_space_location(_rulemap.at("autoindex"));
		autoindex_value = find_value_location(autoindex_rule);
		for (size_t i = 0; i < autoindex_value.length(); i++)
		{
			if (!isalpha(autoindex_value[i]))
				throw std::invalid_argument(" invalid character in autoindex directive");
		}
		if (autoindex_value == "on")
			return (true);
		else if (autoindex_value == "off")
			return (false);
		else
			throw std::invalid_argument(" invalid rule in autoindex directive");
	}
	else 
		return (false);
}

std::string location::validateAlias()
{
	std::string alias_rule;
	std::string alias_value;

	if (_rulemap.contains("alias"))
	{
		alias_rule = normalize_space_location(_rulemap.at("alias"));
		alias_value = find_value_location(alias_rule);
	}
	else 
		return ("");
	for (size_t i = 0; i < alias_value.length(); i++)
	{
		if (!isalpha(alias_value[i]) && !isdigit(alias_value[i]) && alias_value[i] != '/' && alias_value[i] != '_' && alias_value[i] != '-' && alias_value[i] != '.')
			throw std::invalid_argument(" invalid character in alias directive");
	}
	return (alias_value);
}

std::string	location::findLocationName(std::string locationLine)
{
	size_t end = locationLine.rfind('{');
	std::string locationName = locationLine.substr(10, end - 11);
	return (locationName);
}

std::string location::validateLocationName(std::string line)
{
	std::string location_name_rule;
	std::string location_name_value;

	if (!line.empty())
	{
		location_name_rule = normalize_space_location(line);
		location_name_value = findLocationName(location_name_rule);
	}
	size_t size = location_name_value.size();
	if (location_name_value[0] == '/' && location_name_value[size - 1] == '/')
	{
		for (size_t i = 0; i < size; i++)
		{
			if (!isalpha(location_name_value[i]) && !isdigit(location_name_value[i]) && location_name_value[i] != '-' && location_name_value[i] != '/')
				throw std::invalid_argument(" invalid character in location name");
			if (location_name_value[i] == '/' && location_name_value[i + 1] == '/')
				throw std::invalid_argument(" invalid character in location name");
		}
	}
	else
		throw std::invalid_argument(" invalid character in location name");
	return (location_name_value);
}

std::vector<std::string> location::validateIndex()
{
    std::vector<std::string> tmp_vector;
    std::string index_rule, index_value;

    if (_rulemap.contains("index"))
    {
        index_rule = normalize_space_location(_rulemap.at("index"));
        index_value = find_value_location(index_rule);
    }
    else
        return (tmp_vector);

    for (char c : index_value)
    {
        if (!std::isalnum(c) && c != '-' && c != '.' && !std::isspace(c))
            throw std::invalid_argument(" invalid character in index directive");
    }

    std::istringstream iss(index_value);
    std::string word;
    while (iss >> word)
        tmp_vector.push_back(word);

    return (tmp_vector);
}

std::string location::validateReturn()
{
	std::string return_rule;
	std::string return_value;

	if (_rulemap.contains("return"))
	{
		return_rule = normalize_space_location(_rulemap.at("return"));
		return_value = find_value_location(return_rule);
	}
	else 
		return ("");
	for (size_t i = 0; i < return_value.length(); i++)
	{
		if (!isalpha(return_value[i]) && !isdigit(return_value[i]) && return_value[i] != '/' \
			&& return_value[i] != ':' && return_value[i] != '_' && return_value[i] != '-' && return_value[i] != '.')
			throw std::invalid_argument(" invalid character in return directive");
	}
	return (return_value);
}

std::string location::validateRoot()
{
	std::string root_rule;
	std::string root_value;
	size_t dot = 0;

	if (_rulemap.contains("root"))
	{
		root_rule = normalize_space_location(_rulemap.at("root"));
		root_value = find_value_location(root_rule);
	}
	else 
		return ("");

	size_t root_value_lenght = root_value.length();
	if (root_value_lenght == 1)
		throw std::invalid_argument(" invalid root path in root directive");

	for (size_t i = 0; i < root_value_lenght; i++)
	{
		if (root_value[0] == '.')
			dot++;
		else if (root_value[0] != '/' || root_value[root_value_lenght - 1] == '/')
			throw std::invalid_argument(" invalid root path directive:" \
			"please start root with '/' "  + root_value + " or it doesn't have to finish wiht a /");

		if (!isalpha(root_value[i]) && !isdigit(root_value[i]) && root_value[i] != '/' && root_value[i] != '_' && root_value[i] != '-' && dot > 1)
			throw std::invalid_argument(" invalid character in root directive");
	}
	std::filesystem::path rootPath(root_value);

	if (!rootPath.is_absolute())
	    rootPath = std::filesystem::current_path() / rootPath;

	if (!std::filesystem::exists(rootPath))
	    throw std::invalid_argument(" root path does not exist! (" + rootPath.string() + ")");

	if (!std::filesystem::is_directory(rootPath))
	    throw std::invalid_argument(" root path is not a directory! (" + rootPath.string() + ")");
	if (root_value[0] == '/')
		root_value.erase(0 , 1);
	else if (root_value[0] == '.' && root_value[1] == '/')
		root_value.erase(0 , 2);
	return (root_value);
}

std::vector<std::string> location::validateCgiPath()
{
	std::string cgi_path_rule;
	std::string cgi_path_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;
	
	static size_t space = 0;

	if (_rulemap.contains("cgi_path"))
	{
		cgi_path_rule = normalize_space_location(_rulemap.at("cgi_path"));
		cgi_path_value = find_value_location(cgi_path_rule);
	}
	else 
		return (tmp_vector);
	for (size_t i = 0; i < cgi_path_value.length(); i++)
	{
		if (isspace(cgi_path_value[i]))
			i++;
		if (!isalpha(cgi_path_value[i]) && !isdigit(cgi_path_value[i]) && cgi_path_value[i] != '_' && cgi_path_value[i] != '/' && cgi_path_value[i] != '-' && cgi_path_value[i] != '.')
			throw std::invalid_argument(" invalid character in cgi_path directive");
	}
	for (size_t i = 0; i < cgi_path_value.length(); i++)
	{
		if (isspace(cgi_path_value[i]))
			space++;
	}
	if (space == 0)
	{
		tmp_value = cgi_path_value.substr(0, cgi_path_value.length());
		tmp_vector.push_back(tmp_value);
	}
	else
	{
		for (size_t i = 0; i < cgi_path_value.length();)
		{
			while (i < cgi_path_value.length() && isspace(cgi_path_value[i]))
				i++;

			size_t start = i;
			while (i < cgi_path_value.length() && !isspace(cgi_path_value[i]))
				i++;

			if (start < i)
			{
				tmp_value = cgi_path_value.substr(start, i - start);
				tmp_vector.push_back(tmp_value);
			}
		}
	}
	return (tmp_vector);
}

std::vector<std::string> location::validateCgiExt()
{
	std::string cgi_ext_rule;
	std::string cgi_ext_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;
	
	static size_t space = 0;

	if (_rulemap.contains("cgi_ext"))
	{
		cgi_ext_rule = normalize_space_location(_rulemap.at("cgi_ext"));
		cgi_ext_value = find_value_location(cgi_ext_rule);
	}
	else 
		return (tmp_vector);
	for (size_t i = 0; i < cgi_ext_value.length(); i++)
	{
		if (isspace(cgi_ext_value[i]))
			i++;
		if (!isalpha(cgi_ext_value[i]) && !isdigit(cgi_ext_value[i]) && cgi_ext_value[i] != '_' && cgi_ext_value[i] != '/' && cgi_ext_value[i] != '-' && cgi_ext_value[i] != '.')
			throw std::invalid_argument(" invalid character in cgi_extension directive");
	}
	for (size_t i = 0; i < cgi_ext_value.length(); i++)
	{
		if (isspace(cgi_ext_value[i]))
			space++;
	}
	if (space == 0)
	{
		tmp_value = cgi_ext_value.substr(0, cgi_ext_value.length());
		tmp_vector.push_back(tmp_value);
	}
	else
	{
		for (size_t i = 0; i < cgi_ext_value.length();)
		{
			while (i < cgi_ext_value.length() && isspace(cgi_ext_value[i]))
				i++;

			size_t start = i;
			while (i < cgi_ext_value.length() && !isspace(cgi_ext_value[i]))
				i++;

			if (start < i)
			{
				tmp_value = cgi_ext_value.substr(start, i - start);
				tmp_vector.push_back(tmp_value);
			}
		}
	}
	return (tmp_vector);
}
