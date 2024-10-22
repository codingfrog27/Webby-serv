/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/22 17:20:06 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>

class Config
{
	private:
		// Private Attributes
		

	public:
		// Constructors and Destructors
		Config(void);
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

	std::multimap<std::string, std::string> serverBlock;
	std::multimap<std::string, std::string> locationBlock;
	std::vector<std::string> _locationName;
	std::string _locations;
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
	
	Config*	parseConfigFile(const std::string filename);
	void	parseLocationBlock(const std::string &line);
	void	parseServerBlock(const std::string &line);
	int		startParsing(const std::string &line);
	void	createSocketObject(Config &config);
	void	initizalizeServerobj();
	void	printBlockValue(const std::multimap<std::string, std::string> &configFile);
	void	updateValue(const std::string& newValue, Config &config);
};


