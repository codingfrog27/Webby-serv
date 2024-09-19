/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/19 13:33:33 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

Socket::Socket(int t_domain, int t_type, int t_protocol) : _domain(t_domain), _protocol(t_protocol), _type(t_type)
{
    //Identify (name) a socket
    _address.sin_family = AF_INET;					 
	_address.sin_addr.s_addr = htonl(INADDR_ANY); //Fill the struct sockaddr_in
	_address.sin_port = htons(PORT);

    //Create a socket
    _socketFd = socket(_domain, _type, _protocol);

    if (_socketFd < 0)
        std::cerr << RED << "Fail to create a Socket..." << RESET << std::endl;

    std::cout << GREEN << "Parameterized Constructor socket has been called." << RESET << std::endl;
}

Socket::~Socket()
{
    std::cout << RED << "Destructor socket has been called." << RESET << std::endl;
}

void    Socket::createConnection(std::string t_filePath)
{   
    std::string http_header =	"HTTP/1.1 200 OK\r\n"
                            	"Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n";
    
    int addrlen = sizeof(_address); //is used to store the size of the address structure when dealing with socket operations in networking. This ensures that the function accept() operates correctly and does not overwrite memory
    long valread; 
    int  new_socket; //socket prepared to accept the connections

    std::cout << "This is the socketfd: " << _socketFd << std::endl;
    //This function associates a socket with a local address and port number. 
    //It tells the operating system that the socket should be used for communication at the specified address and port
    if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
        std::cerr << RED << "Fail to bind the Socket..." << RESET << std::endl;

    //This function configures a socket to listen for incoming connection requests from clients. 
    //After binding a socket to an address and port, we use listen() to indicate that the socket is ready to accept incoming connections.
    if (listen(_socketFd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
    }

    while (1)
    {
        std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
        // This function is called to accept an incoming connection request on a socket that has been set up to listen for connections. 
        //When a client attempts to connect to the server, accept() creates a new socket for that connection and establishes the communication channel.
        if ((new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0)
		{
            std::cerr << RED << "It has yet to be accepted..." << RESET << std::endl;
		}

        //Send and receive messages
		char buffer[1024] = {0};

        //store the number of bytes read from the read() function call
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
