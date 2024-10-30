/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/30 11:41:31 by asimone          ###   ########.fr       */
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
		// Constructors and Destructors
		Config(void);
		Config(std::ifstream &file, std::string &line);
		Config(const Config &rhs);
		Config &operator=(const Config &rhs);
		~Config(void);

	
	// std::map<std::string, std::string> _configMap;

	//these are all REQUIRED
	size_t i = 0;
	std::string	_serverPort;
	// size_t		_serverPort;
	std::string	_serverName;
	size_t		_maxConnects;
	size_t		_timeout; //general _timeout waiting for request/respond sending
	std::string	_rootDir;
	std::string	_autoIndexFilePath;

	std::unordered_map<std::string, location> _locations;
	std::vector<location> 	_newLocations;
	std::vector<std::string> _locNames;
	std::map<std::string, std::string> _rulemap;
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
	
	std::vector<Config>	parseConfigFile(const std::string fileName);
	void	parseLocationBlock(std::ifstream &file, const std::string &locationName);
	void	parseServerBlock(const std::string &line);
	int		startParsing(const std::string &line);
	void	createSocketObject(Config &config);
	void	initizalizeServerobj();
	void	updateValue(const std::string& newValue, Config &config);
	void	findKeyandValue(const std::string &line, std::multimap<std::string, std::string> &block);
	
	
	location findLocation (const std::multimap<std::string, location> & locations, const std::string& locationName);
	void	printBlockValue(const std::multimap<std::string, std::string> &configFile);
	void	parseRule(const std::string &line);
};


