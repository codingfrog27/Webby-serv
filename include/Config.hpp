/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/04 15:48:08 by asimone          ###   ########.fr       */
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
#include "location.hpp"

class Config
{
	private:
		// Private Attributes
		

	public:
		std::string _autoindex;
		std::string _autoIndexFilePath;
		std::string _client_max_body_size;
		std::string _errorPage;
		std::string _host;
		std::string _index;
		std::string _listen;
		std::string _rootDir;
		std::string _serverPort;
		std::string _serverName;
		size_t		_maxConnects;
		size_t		_timeout; //general _timeout waiting for request/respond sending
		std::vector<location> 	_newLocations;
		std::vector<std::string> _locNames;
		std::unordered_map<std::string, location> _locations;
		std::unordered_map<std::string, std::string> _rulemap;

		// Constructors and Destructors
		Config(void);
		Config(std::ifstream &file, std::string &line);
		Config(const Config &rhs);
		Config &operator=(const Config &rhs);
		~Config(void);

	
	// std::map<std::string, std::string> _configMap;

	//these are all REQUIRED
	size_t i = 0;
	// size_t		_serverPort;
	// std::vector<std::string> _locationName;
	// std::vector<location> _location;
	// std::vector <location> _locations;
	// std::vector <Socket> _server;
	// std::string	error_log_file;
	// std::string	access_log_file;

			
	// size_t		keep_alive_timeout; //spefically how long to keep an idle connection open before closing
	// bool		directory_listing; //aka autoindex: wether to display list of pages when requested dir isnt found
	// std::string	cgi_script_directory;
	// // not required but expected

	// size_t		max_keep_alive_requests;
	// std::unordered_map<std::string, std::string> custom_error_pages;
	// std::unordered_map<std::string, std::string> access_control;
	
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
	void		initializeServer();
	std::string toString() const;
};
	
std::vector<Config>	parseConfigFile(const std::string fileName);
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);
bool	checkstr(const std::string &line, const std::string &str);


