/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/19 14:58:32 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

Socket::Socket(int t_domain, int t_type, int t_protocol) : _domain(t_domain), _protocol(t_protocol), _type(t_type)
{
    struct addrinfo hints, *p, *servinfo;
    
    // Initialize hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = _domain; //don't care IPv4 or IPv6
    hints.ai_socktype = _type; //TCP stream sockets
    hints.ai_flags = AI_PASSIVE; //fill in my IP for me
    hints.ai_protocol = _protocol;
    
    //Identify (name) a socket
    _address.sin_family = AF_INET;					 
	_address.sin_addr.s_addr = htonl(INADDR_ANY); //Fill the struct sockaddr_in
	_address.sin_port = htons(PORT);
 
    // Get address information
    int status = getaddrinfo(NULL, "8080", &hints, &servinfo);
    if (status != 0)
    {
        std::cerr << RED << "getaddrinfo error: " << gai_strerror(status) << RESET << std::endl;
        return;
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        //Create a socket
        _socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(_socketFd < 0)
        {
            std::cerr << RED << "Fail to create a Socket..." << RESET << std::endl;
            continue;
        }

        // Set socket to non-blocking
        // if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
        // {
        //     std::cerr << RED << "Failed to set socket to non-blocking..." << RESET << std::endl;  //TO BE FIX!!!!
        //     close(_socketFd);
        //     continue;
        // }
        
        //This function associates a socket with a local address and port number. 
        //It tells the operating system that the socket should be used for communication at the specified address and port
        if (bind(_socketFd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_socketFd);
            std::cerr << RED << "Bind failed with error: " << strerror(errno) << RESET << std::endl;
            continue;
        }

        break; //Successfully bound, exit the loop
    }

    if (p == NULL)
        std::cerr << RED << "Fail to bind the Socket..." << RESET << std::endl;

    freeaddrinfo(servinfo); //Free the linked list
    
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
    
    int addrlen = sizeof(_address); //Is used to store the size of the address structure when dealing with socket operations in networking. This ensures that the function accept() operates correctly and does not overwrite memory
    long valread; 
    int  new_socket; //Socket prepared to accept the connections

    //This function configures a socket to listen for incoming connection requests from clients. 
    //After binding a socket to an address and port, we use listen() to indicate that the socket is ready to accept incoming connections.
    if (listen(_socketFd, 3) < 0)
    {
        std::cerr << RED << "Listen failed with error: " << strerror(errno) << RESET << std::endl;
    }

    while (1)
    {
        std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
        // This function is called to accept an incoming connection request on a socket that has been set up to listen for connections. 
        //When a client attempts to connect to the server, accept() creates a new socket for that connection and establishes the communication channel.
        if ((new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0)
		{
            std::cerr << RED << "Accept failed with error: " << strerror(errno) << RESET << std::endl;
            break;
		}

        //Send and receive messages
		char buffer[1024] = {0};

        //Store the number of bytes read from the read() function call
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
			
			//Send the HTTP header
			write(new_socket, http_header.c_str(), http_header.size());
			//Send the client file content
			write(new_socket, file_content.c_str(), file_content.size());
			std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;
		}
		else
			std::cerr << RED << "Could not open file!" << RESET << std::endl;

		close(new_socket);
    }
}
