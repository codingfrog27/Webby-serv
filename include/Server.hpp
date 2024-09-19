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
	struct sockaddr_in			_address;
	int							_domain;
	int							_protocol;
	int							_serverFd;
	int							_type;

public:
	//ORTHODOX CANONICAL CLASS FORM//
	Server(int t_domain, int t_type, int t_protocol);
	// Server(const Server &obj);
	// Server& operator=(const Server& obj);
	~Server();

	void	createConnection(std::string t_filePath);
};