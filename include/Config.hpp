/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/07 15:51:51 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <memory>
#include "location.hpp"

enum ServerRules
{
	LISTEN,
	MAX_BODY_SIZE,
	ERROR_PAGE,
	HOST,
	INDEX,
	ROOT,
	SERVER_NAME,
	EMPTY
};

class Config
{
	public:
		std::string _autoindex;
		std::string _autoIndexFilePath;
		std::string _client_max_body_size;
		std::string _errorPage;
		std::string _host;
		std::string _index;
		std::string _listen;
		std::string _rootDir;
		std::string _serverName;
		std::string _serverPort;
		size_t		_maxConnects;
		size_t		_timeout; //general _timeout waiting for request/respond sending
		
		std::vector<std::unique_ptr<location>> _newLocations;
		std::vector<std::string> _locNames;
		std::unordered_map<std::string, location> _locations;
		std::unordered_map<std::string, std::string> _rulemap;

		// Constructors and Destructors
		Config(void);
		Config(std::ifstream &file, std::string &line);
		Config(const Config &rhs);
		Config &operator=(const Config &rhs);
		~Config(void);

	

	//these are all REQUIRED
	size_t i = 0;
	
	
	private:
	location findLocation (const std::multimap<std::string, location> & locations, const std::string& locationName);
	void	printBlockValue(const std::multimap<std::string, std::string> &configFile);
	void	parseRule(const std::string &line);
	
	std::string	getErrorPage();
	std::string	getMaxBodySize();
	std::string	getHost();
	std::string	getIndex();
	std::string	getListen();
	std::string	getRoot();
	std::string	getServerName();
	void		setErrorPage(const std::string &key);
	void		setHost(const std::string &key);
	void		setIndex(const std::string &key);
	void		setListen(const std::string &key);
	void		setMaxBodySize(const std::string &key);
	void		setRoot(const std::string &key);
	void		setServerName(const std::string &key);
	int		mapToMembers();
	void	setServer(const int rule);

	std::string	validateListen();
	std::string toString() const;
};


std::ostream& operator<<(std::ostream& os, const Config& config);
std::vector<Config>	parseConfigFile(const std::string fileName);
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);
bool	checkstr(const std::string &line, const std::string &str);


