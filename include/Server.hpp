/* ************************************************************************** */
/*	   */
/*	:::	  ::::::::   */
/*   Server.hpp	   :+:	  :+:	:+:   */
/*	+:+ +:+	 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>	  +#+  +:+	   +#+	*/
/*	  +#+#+#+#+#+   +#+	   */
/*   Created: 2024/08/22 18:42:27 by mde-cloe	  #+#	#+#	   */
/*   Updated: 2024/08/22 18:42:58 by mde-cloe	 ###   ########.fr	   */
/*	   */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include "Location.hpp"
#include <map>
#include <netinet/in.h>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <vector>

#define CYAN	"\033[36m"
#define GREEN	"\033[32m"
#define MAGENTA "\033[35m"
#define RED	 "\033[31m"
#define YELLOW	"\033[33m"
#define RESET	"\033[0m"

#define PORT 8080 // common used port for web servers

class Server
{
private:

	struct sockaddr_in 			m_address;
	int							m_addressFamily;
	std::string	 				m_clientMaxBodySize; //Maximum size of the client request body that the server will accept 
	std::map<int, std::string>	m_errorPages; //HTTP status codes to custom error page files
	u_long						m_host;
	std::string 				m_index; //Default index file(s) to be served when a directory is requested 
	std::vector<Location>		m_locations; //Location object defines how requests matching a particular URL path should be handled
	int							m_port; //Port number on which the server listens for incoming connections
	int							m_protocol;
	std::string					m_root; //Root directory from which files are served
	int							m_serverFd;
	std::string					m_serverName; //Server’s name or hostname for matching requests;
	int							m_type;

	//std::string					m_accessLog; //Path to the file where access logs are written
	//bool						m_autoIndex; //Directory indexing is enabled for the server
	//std::string 				m_errorLog; //Path to the file where error logs are written	 
	//std::string					m_redirect; //URL to which all requests to the server should be redirected 

public:
	//ORTHODOX CANONICAL CLASS FORM//
	//Server();
	//Server(const Server &obj);
	//Server& operator=(const Server& obj);
	Server(int t_addressFamily, std::string t_clientMaxBodySize, std::map<int, std::string> t_errorPages, u_long t_host, std::string t_index, std::vector<Location>	t_locations, int t_port, int t_protocol, std::string t_root, std::string t_serverName, int t_type);
	~Server();

	//GET FUNTIONS//
	//const std::string &getAccessLog();
	//const bool &getAutoIndex();
	//const std::string &getClientMaxBodySize();
	//const std::map<int, std::string> &getErrorPages();
	//const std::string &getErrorLog();
	//const std::string &getIndex();
	//const std::vector<Location> &getLocations();
	//const int &getPort();
	//const std::string &getRedirect();
	//const std::string &getRoot();
	//const std::string &getServerName();

	//SET FUNCTIONS//
	//void setAccessLog(const std::string &t_accessLog);
	//void setAutoIndex(const bool &t_autoIndex);
	//void setClientMaxBodySize(const std::string &t_clientMaxBodySize);
	//void setErrorPages(const std::map<int, std::string> &t_errorPages);
	//void setErrorLog(const std::string &t_errorLog);
	//void setIndex(const std::string &t_index);
	//void setLocations(const std::vector<Location> &t_locations);
	//void setPort(const int &t_port);
	//void setRedirect(const std::string &t_redirect);
	//void setRoot(const std::string &t_root);
	//void setServerName(const std::string &t_serverName);


	//PARSER FUNCTIONS//
	//void searchValue(std::string& fileToRead); //, std::string valueToParse, std::string buffer);
	//void webServer(std::string fileToParse);
	//void keyAnalisize();
};