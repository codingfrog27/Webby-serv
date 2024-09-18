/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: antoniosimone <antoniosimone@student.42      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/16 15:06:45 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/09/17 19:39:45 by antoniosimo   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int t_addressFamily, std::string t_clientMaxBodySize, std::map<int, std::string> t_errorPages, u_long t_host, std::string t_index, std::vector<Location>	t_locations, int t_port, int t_protocol, std::string t_root, std::string t_serverName, int t_type) : 
m_addressFamily(t_addressFamily), m_clientMaxBodySize(t_clientMaxBodySize), m_errorPages(t_errorPages), m_host(t_host), m_index(t_index), m_locations(t_locations), m_port(t_port), m_protocol(t_protocol), m_root(t_root), m_serverName(t_serverName), m_type(t_type)
{
	//Identify (name) a socket
	m_address.sin_family = m_addressFamily;
	m_address.sin_port = htons(m_port);			//Fill the struct sockaddr_in 
	m_address.sin_addr.s_addr = htonl(m_host);

	//Create a socket
	m_serverFd = socket(m_addressFamily, m_type, m_protocol);
	
	std::cout << GREEN << "Parameterized Constructor Server has been called." << RESET << std::endl;
}

Server::~Server()
{
	std::cout << RED << "Default Destructor Server has been called." << RESET << std::endl;
}