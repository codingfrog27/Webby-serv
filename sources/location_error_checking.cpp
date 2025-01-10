#include "location.hpp"

std::string	find_value_location(std::string& directive)
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

std::vector<std::string> location::validateAllowMethods()
{
	std::string allow_methods_rule;
    std::string allow_methods_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;
	
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
			throw std::invalid_argument("Error: invalid character in allow_methods directive");
	}
	for (size_t i = 0; i < allow_methods_value.length(); i++)
	{
		if (isspace(allow_methods_value[i]))
			space++;
	}
	if (space == 0)
	{
		tmp_value = allow_methods_value.substr(0, allow_methods_value.length());
		if (tmp_value == "POST" || tmp_value == "GET" || tmp_value == "DELETE") //|| isspace(tmp_value[space]) == 0)
			tmp_vector.push_back(tmp_value);
		else
		 	throw std::invalid_argument("Error: invalid ruleeeee in allow_methods directive");
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
				if (tmp_value == "POST" || tmp_value == "GET" || tmp_value == "DELETE")
					tmp_vector.push_back(tmp_value);
				else
					throw std::invalid_argument("Error: invalid rule in autoindex directive");
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
				throw std::invalid_argument("Error: invalid character in autoindex directive");
		}
		if (autoindex_value == "on")
			return (true);
		else if (autoindex_value == "off")
			return (false);
		else
			throw std::invalid_argument("Error: invalid rule in autoindex directive");
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
			throw std::invalid_argument("Error: invalid character in alias directive");
	}
	return (alias_value);
}

std::vector<std::string>		location::validateIndex()
{
	std::string index_rule;
	std::string index_value;
	std::string tmp_value;
	std::vector<std::string>  tmp_vector;
	
	static int space = 0;

	if (_rulemap.contains("index"))
	{
		index_rule = normalize_space_location(_rulemap.at("index"));
		index_value = find_value_location(index_rule);
	}
	else
		return (tmp_vector);
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
	// for (size_t i = tmp_vector.begin(); i < tmp_vector.end(); i++)
	// 	std::cout << *i << std::endl;
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
		if (!isalpha(return_value[i]) && !isdigit(return_value[i]) && return_value[i] != '/' && return_value[i] != ':' && return_value[i] != '_' && return_value[i] != '-' && return_value[i] != '.')
		 	throw std::invalid_argument("Error: invalid character in return directive");
	}
	return (return_value);
}

std::string location::validateRoot()
{
	std::string root_rule;
	std::string root_value;

	if (_rulemap.contains("root"))
	{
		root_rule = normalize_space_location(_rulemap.at("root"));
		root_value = find_value_location(root_rule);
	}
	else 
		return ("");
	for (size_t i = 0; i < root_value.length(); i++)
	{
		if (!isalpha(root_value[i]) && !isdigit(root_value[i]) && root_value[i] != '/' && root_value[i] != '_' && root_value[i] != '-' && root_value[i] != '.')
			throw std::invalid_argument("Error: invalid character in root directive");
	}
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
			throw std::invalid_argument("Error: invalid character in cgi_path directive");
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
			throw std::invalid_argument("Error: invalid character in cgi_extension directive");
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
