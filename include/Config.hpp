/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/02/05 18:47:04 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "external.hpp"
#include "location.hpp"
#include "Colors.hpp"

#define MAX_TIMEOUT 60000

class Config
{
	public:
		bool 									_autoindex;
		std::string 							_autoIndexFilePath;
		std::string 							_client_max_body_size;
		std::multimap<std::string, std::string> _errorPage;
		std::string 							_host;
		std::string 							_listen;
		std::string 							_rootDir;
		std::string 							_serverName;
		std::string 							_serverPort;
		size_t									_timeout; 
		std::vector<std::string> 				_index;
		std::vector<location> 					_locations;

		// Constructors and Destructors
		Config(void);
		Config(std::ifstream &file, std::string &line);
		Config(const Config &rhs);
		Config &operator=(const Config &rhs);
		~Config(void);

		//getters and setters
		bool		getAutoindex();
		std::multimap<std::string, std::string> getErrorPage();
		std::string	getMaxBodySize();
		std::string	getHost();
		std::vector<std::string> getIndex();
		std::string	getListen();
		std::string	getRoot();
		size_t		getTimeout();
		std::string	getServerName();

	class NoBlockFound : public std::exception
	{
		public:
			std::string _errMsg;
		NoBlockFound(std::string errMsg);
		const char		*what() const noexcept override;
	};

	//these are all REQUIRED
	
	
	private:
	size_t i = 0;
		std::unordered_map<std::string, std::string> _rulemap;
	void		readBlock(std::ifstream &file, std::string &line);
	location	findLocation (const std::multimap<std::string, location> & locations, const std::string& locationName);
	void		parseRule(const std::string &line);
	void		setAutoindex(const bool& autoIndex);
	void		setErrorPage(const std::multimap<std::string, std::string> &errorPageMap);
	void		setHost(const std::string &host);
	void		setIndex(const std::vector<std::string> &index_vector);
	void		setListen(const std::string &listen);
	void		setMaxBodySize(const std::string &maxBodySize);
	void		setRoot(const std::string &root);
	void		setTimeout(const size_t& timeout);
	void		setServerName(const std::string &serverName);
	void		setServer(const int rule);

	int			initializeServer();
	void			mapToMembers();
	bool		validateAutoindex();
	std::multimap<std::string, std::string> validateErrorPage();
	std::string validateHost();
	std::vector<std::string> ValidateIndex();
	std::string	validateListen();
	std::string validateMaxBodySize();
	std::string validateRoot();
	size_t 		validateTimeout();
	std::string validateServerName();
};

void	parseConfig(int argc, char ** argv, std::vector<Config> &configs);
void	checkPortUniqueness(const std::vector<std::unique_ptr<Config>> &configs);
void	printConfigs(std::vector<Config> &configs);
std::vector<Config>	readConfigFile(const std::string fileName);
bool	checkCaracter(const std::string &line, const char &c);
bool	checkstr(const std::string &line, const std::string &str);
bool	locationFound(std::string &line);

void	print_map(const std::multimap<std::string, std::string> map);
std::string	normalize_space(std::string& str);
std::ostream& 		operator<<(std::ostream& os, const Config& config);
