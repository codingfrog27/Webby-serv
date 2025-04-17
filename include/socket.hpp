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
#include <fcntl.h>
#include <fstream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <vector>


#include "Config.hpp"

#define CYAN	"\033[36m"
#define GREEN	"\033[32m"
#define MAGENTA "\033[35m"
#define RED	 	"\033[31m"
#define YELLOW	"\033[33m"
#define RESET	"\033[0m"

class Socket
{
	private:
		const struct addrinfo	*_addrInfo;
		int						_setYes;
		struct sockaddr_storage _address;
		std::string				_hostname; 
		std::string				_port;
		char ip_address[INET6_ADDRSTRLEN];

		void					openSocket();
		void 					*get_in_addr(struct sockaddr *sa);


	public:
		int						_socketFd;
							Socket(Config *config, const struct addrinfo *addressInfo);
							~Socket();
		int					createConnection();
};
	
void sendHTMLPage(int client_socket, const std::string& file_path);