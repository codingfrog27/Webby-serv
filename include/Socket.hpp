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

class Socket
{
// private:
// 	std::string	 				_clientMaxBodySize; //Maximum size of the client request body that the server will accept 
// 	std::map<int, std::string>	_errorPages; //HTTP status codes to custom error page files
// 	std::string 				_index; //Default index file(s) to be served when a directory is requested 
// 	std::vector<Location>		_locations; //Location object defines how requests matching a particular URL path should be handled
// 	std::string					_root; //Root directory from which files are served
// 	std::string					_socketName; //Server’s name or hostname for matching requests;

protected:
	struct sockaddr_in 			_address;
	int							_addressFamily;
	int 						_connection;
	u_long						_host;
	int							_port; //Port number on which the server listens for incoming connections
	int							_protocol;
	int							_SocketFd;
	int							_type;


	//std::string				_accessLog; //Path to the file where access logs are written
	//bool						_autoIndex; //Directory indexing is enabled for the server
	//std::string 				_errorLog; //Path to the file where error logs are written	 
	//std::string				_redirect; //URL to which all requests to the server should be redirected 

public:
	//ORTHODOX CANONICAL CLASS FORM//
	//Server();
	//Server(const Server &obj);
	//Server& operator=(const Server& obj);
	Socket(int t_addressFamily, u_long t_host, int t_port, int t_protocol, int t_type);
	~Socket();

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
	//const std::string &getSocketName();

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


	//Virtual function to connect to a network
	virtual int connectToNetwork(int socketFd, struct sockaddr_in address) = 0;
	//Function to test sockets and connection
	void test_connection(int test);

	//SETTER FUNCTION
	void setConnection(int connect);

	//GETTER FUNCTION
	const struct sockaddr_in& getaddress();
	const int &getconnection();
	const int &getsocketFd();
};