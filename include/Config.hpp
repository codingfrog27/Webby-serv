/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/05 16:22:05 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <exception>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <memory>
#include "location.hpp"
#include "Colors.hpp"

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
		std::unordered_map<std::string, std::string> _errorPage;
		std::string _host;
		std::vector<std::string> _index;
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
	std::string toString() const;
	std::unordered_map<std::string, std::string> getErrorPage();
	std::string	getMaxBodySize();
	std::string	getHost();
	std::vector<std::string> getIndex();
	std::string	getListen();
	std::string	getRoot();
	std::string	getServerName();

	class NoBlockFound : public std::exception
	{
		public:
			std::string _errMsg;
		NoBlockFound(std::string errMsg);
		const char		*what() const noexcept override;
	};

	//these are all REQUIRED
	size_t i = 0;
	
	
	private:
	void		readBlock(std::ifstream &file, std::string &line);
	location	findLocation (const std::multimap<std::string, location> & locations, const std::string& locationName);
	void		printBlockValue(const std::multimap<std::string, std::string> &configFile);
	void		parseRule(const std::string &line);
	void		setErrorPage(const std::unordered_map<std::string, std::string> &errorPageMap);
	void		setHost(const std::string &host);
	void		setIndex(const std::vector<std::string> &index_vector);
	void		setListen(const std::string &listen);
	void		setMaxBodySize(const std::string &maxBodySize);
	void		setRoot(const std::string &root);
	void		setServerName(const std::string &serverName);
	void		setServer(const int rule);

	int			initializeServer();
	void		mapToMembers();
	std::unordered_map<std::string, std::string> validateErrorPage();
	std::string validateHost();
	std::vector<std::string> ValidateIndex();
	std::string	validateListen();
	std::string validateMaxBodySize();
	std::string validateRoot();
	std::string validateServerName();
};

void	parseConfig(int argc, char ** argv, std::vector<Config> &configs);
void	checkPortUniqueness(const std::vector<std::unique_ptr<Config>> &configs);
void	printConfigs(const std::vector<Config> &configs);
std::vector<Config>	readConfigFile(const std::string fileName);
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);
bool	checkstr(const std::string &line, const std::string &str);

void	print_map(const std::unordered_map<std::string, std::string> map);
std::string	normalize_space(std::string& str);
std::ostream& 		operator<<(std::ostream& os, const Config& config);
