/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/19 12:28:39 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int t_domain, int t_type, int t_protocol) : _domain(t_domain), _protocol(t_protocol), _type(t_type)
{
    //Identify (name) a socket
    _address.sin_family = AF_INET;					 
	_address.sin_addr.s_addr = htonl(INADDR_ANY); //Fill the struct sockaddr_in
	_address.sin_port = htons(PORT);

    //Create a socket
    _serverFd = socket(_domain, _type, _protocol);

    if (_serverFd < 0)
        std::cerr << RED << "Fail to create a Socket..." << RESET << std::endl;

    std::cout << GREEN << "Parameterized Constructor Server has been called." << RESET << std::endl;
}

Server::~Server()
{
    std::cout << RED << "Destructor Server has been called." << RESET << std::endl;
}

void    Server::createConnection(std::string t_filePath)
{   
    std::string http_header =	"HTTP/1.1 200 OK\r\n"
                            	"Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n";
    
    int addrlen = sizeof(_address);
    long valread; 
    int  new_socket; //socket prepared to accept the connections

    std::cout << "This is the serverfd: " << _serverFd << std::endl;
    
    if (bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
        std::cerr << RED << "Fail to bind the Socket..." << RESET << std::endl;

    //On the server, wait for an incoming connection
    if (listen(_serverFd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
    }

    while (1)
    {
        std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
        if ((new_socket = accept(_serverFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0)
		{
            std::cerr << RED << "It has yet to be accepted..." << RESET << std::endl;
		}

        //Send and receive messages
		char buffer[1024] = {0};

		valread = read(new_socket, buffer, 1024);
		if (valread < 0)
		{
			std::cerr << RED << "No bytes are there to read" << RESET << std::endl;
		}
        
        std::ifstream 	file(t_filePath);
		if (file.is_open())
		{
			std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
			
			//send the HTTP header
			write(new_socket, http_header.c_str(), http_header.size());
			//send the client file content
			write(new_socket, file_content.c_str(), file_content.size());
			std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;
		}
		else
			std::cerr << RED << "Could not open file!" << RESET << std::endl;

		close(new_socket);
    }
}
