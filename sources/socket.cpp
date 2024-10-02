/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/02 14:39:53 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include "Client.hpp"

Socket::Socket(const std::string &t_hostname, const std::string &t_port) : _hostname(t_hostname), _port(t_port)
{
    struct addrinfo hints, *p, *servinfo;
    
    //Initialize hints. Hold information about address and port details
    memset(&hints, 0, sizeof(hints)); //Clear the struct first
    hints.ai_family = AF_UNSPEC; //Specifies the address family. AF_UNSPEC = don't care IPv4 or IPv6
    hints.ai_flags = AI_PASSIVE; //Provides additional options (AI_PASSIVE for binding to all network interfaces)
    hints.ai_protocol = IPPROTO_TCP; //Specifies the protocol
    hints.ai_socktype = SOCK_STREAM; //Specifies the socket type (SOCK_STREAM for TCP)
 
    //Used to obtain address information for a given hostname and/or service
    int status = getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &servinfo); 
    if (status != 0)
    {
        std::cerr << RED << "getaddrinfo error: " << gai_strerror(status) << RESET << std::endl;
        return;
    }
    //getaddrinfo might return multiple address structures because there could be multiple valid ways to connect to the desired host
    //The loop breaks as soon as a socket is successfully created and bound, 
    //or it continues until the end of the list if none of the address structures work
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        //Create a new socket for network communication
        _socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(_socketFd < 0)
        {
            std::cerr << RED << "Fail to create a Socket." << RESET << std::endl;
            continue;
        }

        // Set socket to non-blocking
        if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
        {
            std::cerr << RED << "Failed to set socket to non-blocking..." << RESET << std::endl;  //TO BE FIX!!!!
            close(_socketFd);
            continue;
        }
        
        int yes = 1;
        //It allows to configure specific behaviors for a socket and avoid the bind function fails, claiming “Address already in use.”
        if (setsockopt(_socketFd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1)
        {
            std::cerr << RED << "Setsockopt failed with error: " << strerror(errno) << RESET << std::endl;
            continue;
        } 

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
        std::cerr << RED << "Fail to bind the Socket." << RESET << std::endl;

    freeaddrinfo(servinfo); //Free the linked list servinfo to avoid memory leaks
    
    std::cout << GREEN << "Parameterized Constructor socket has been called." << RESET << std::endl;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Socket::~Socket()
{
    std::cout << RED << "Destructor socket has been called." << RESET << std::endl;
}

void sendHTMLPage(int client_socket, const std::string& file_path) 
{
    //Open the HTML file
    std::ifstream file(file_path);
    if (!file) 
    {
        std::cerr << RED << "Error opening file: " << file_path << RESET << std::endl;
        return;
    }

    //Read the file content, store it in buffer and covert it into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string html_content = buffer.str();

    //HTTP Response Headers
    std::string http_response = "HTTP/1.1 200 OK\r\n";
    http_response += "Content-Type: text/html\r\n";
    http_response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
    http_response += "Connection: close\r\n";
    http_response += "\r\n";

    //Send the HTTP header
    send(client_socket, http_response.c_str(), http_response.size(), 0);
    //Send the client file content
    send(client_socket, html_content.c_str(), html_content.size(), 0);
    std::cout << YELLOW << "--------- HTML message sent ----------" << RESET << std::endl;

    file.close();
}

void    Socket::createConnection(std::string t_filePath)
{   
    socklen_t   addrlen = sizeof(_address);
    std::vector <struct pollfd> pfds;   
	std::vector <struct Client> clients;
    
    int         new_socket; 
    int         pollin_happened;
    int         num_events;

    pfds[0].fd = _socketFd; 
    pfds[0].events = POLLIN | POLLOUT;

    if (listen(_socketFd, BACKLOG) < 0)
        std::cerr << RED << "Listen failed with error: " << strerror(errno) << RESET << std::endl;
        
    while (1)
    {
        std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
        
        new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
        if (new_socket >= 0)
        {
            std::cout << GREEN << "New connection accepted" << RESET << std::endl;
            
		    char ip_address[INET6_ADDRSTRLEN] = {0};
            
            inet_ntop(_address.ss_family, get_in_addr((struct sockaddr *)&_address), ip_address, sizeof ip_address);
            std::cout << CYAN << "server: got connection from " << ip_address << RESET << std::endl;
            
            pfds.push_back({new_socket, POLLIN | POLLOUT, 0});
            Clients.push_back((new_socket));
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // pollin_happened = manageConnection(new_socket);
            std::cerr << CYAN << "No connections available, retrying..." << RESET << std::endl;
            continue;
        }
        else
        {
            std::cerr << RED << "Accept failed with error: " << strerror(errno) << RESET << std::endl;
            // break; //???
        }
        
        num_events = poll(pfds.data(), pfds.size(), 2500);

        if (num_events < 0) 
		{
			std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
			break;
		}
		else if (num_events == 0)
		{
			std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
			continue;
		}
		for (size_t i = 0; i < pfds.size(); ++i)
		{
			if (pfds[i].revents & POLLIN)
            {
                Client& client = clients[pfds[i].fd];
				client.req->main_reader(pfds[i].fd);
            }
			if ((pfds[i].revents & POLLOUT) && .doneReading)
				// Clients[i].write_response();
                sendHTMLPage(new_socket, t_filePath); //Send the HTML page with the new socket
			// if (!Clients[i].keep_open)
                close(new_socket); // close connection and remove from vectors;
		}
    }
}




// int    Socket::manageConnection(int socketFd)
// {
   
//  //change con
    


//     //This system call monitors multiple file descriptors (in this case, just one) to see if any of them have events that need to be handled
    


// 	while (1)
// 	{
// 		// check for new connects, if yes expand our vectors
		
        

// 		/* code */
// 	}
    
//     if (pfds[0].revents & POLLIN)
//         return (1); 
//     return (0);
// }
