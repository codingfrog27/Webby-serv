// /* ************************************************************************** */
// /*																			*/
// /*														:::	  ::::::::   */
// /*   socket.cpp										 :+:	  :+:	:+:   */
// /*													+:+ +:+		 +:+	 */
// /*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
// /*												+#+#+#+#+#+   +#+		   */
// /*   Created: 2024/08/16 15:06:45 by mde-cloe		  #+#	#+#			 */
// /*   Updated: 2024/10/07 18:29:51 by mde-cloe		 ###   ########.fr	   */
// /*																			*/
// /* ************************************************************************** */

// #include "socket.hpp"

// // Socket::Socket(const std::string &t_hostname, const std::string &t_port)
// //  : _hostname(t_hostname), _port(t_port)
// Socket::Socket(Config *config)
//  : _hostname(config->_serverName), _port(config->_serverPort)
// {
// 	struct addrinfo hints, *p, *servinfo;
// 	memset(&hints, 0, sizeof(hints)); //Clear the struct first
// 	hints.ai_family = AF_UNSPEC; //Specifies the address family. AF_UNSPEC = don't care IPv4 or IPv6
// 	hints.ai_flags = AI_PASSIVE; //Provides additional options (AI_PASSIVE for binding to all network interfaces)
// 	hints.ai_protocol = IPPROTO_TCP; //Specifies the protocol
// 	hints.ai_socktype = SOCK_STREAM; //Specifies the socket type (SOCK_STREAM for TCP)

 
// 	//Used to obtain address information for a given hostname and/or service
// 	int status = getaddrinfo(_hostname.c_str(), _port.c_str(), &hints, &servinfo); 
// 	if (status != 0)
// 	{
// 		std::cerr << RED << "getaddrinfo error: " << gai_strerror(status) << RESET << std::endl;
// 		return;
// 	}
// 	//getaddrinfo might return multiple address structures because there could be multiple valid ways to connect to the desired host
// 	//The loop breaks as soon as a socket is successfully created and bound, 
// 	//or it returns until the end of the list if none of the address structures work
// 	for(p = servinfo; p != NULL; p = p->ai_next)
// 	{
// 		//Create a new socket for network communication
// 		_socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
// 		if(_socketFd < 0)
// 		{
// 			std::cerr << RED << "Fail to create a Socket." << RESET << std::endl;
// 			return;
// 		}

// 		// Set socket to non-blocking
// 		if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
// 		{
// 			std::cerr << RED << "Failed to set socket to non-blocking..." << RESET << std::endl;  //TO BE FIX!!!!
// 			close(_socketFd);
// 			return;
// 		}
		
// 		int yes = 1;
// 		//It allows to configure specific behaviors for a socket and avoid the bind function fails, claiming “Address already in use.”
// 		if (setsockopt(_socketFd, SOL_SOCKET,SO_REUSEADDR ,&yes, sizeof(yes)) == -1)
// 		{
// 			std::cerr << RED << "Setsockopt failed with error: " << strerror(errno) << RESET << std::endl;
// 			return;
// 		} 
// 		if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) == -1)
// 		{
// 			std::cerr << RED << "Setsockopt SO_REUSEPORT failed with error: " << strerror(errno) << RESET << std::endl;
// 			close(_socketFd);
// 			return;
// 		}
// 		//This function associates a socket with a local address and port number. 
// 		//It tells the operating system that the socket should be used for communication at the specified address and port
// 		if (bind(_socketFd, p->ai_addr, p->ai_addrlen) == -1)
// 		{
// 			close(_socketFd);
// 			std::cerr << RED << "Bind failed with error: " << strerror(errno) << RESET << std::endl;
// 			return;
// 		}
		
// 		break; //Successfully bound, exit the loop
// 	}

// 	if (p == NULL)
// 		std::cerr << RED << "Fail to bind the Socket." << RESET << std::endl;

// 	freeaddrinfo(servinfo); //Free the linked list servinfo to avoid memory leaks
	
// 	std::cout << GREEN << "Parameterized Constructor socket has been called." << RESET << std::endl;
// 	if (listen(_socketFd, BACKLOG) < 0)
// 		std::cerr << RED << "Listen failed with error: " << strerror(errno) << RESET << std::endl;
// }

// void *get_in_addr(struct sockaddr *sa)
// {
// 	if (sa->sa_family == AF_INET)
// 		return &(((struct sockaddr_in*)sa)->sin_addr);
// 	return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

// Socket::~Socket()
// {
// 	std::cout << RED << "Destructor socket has been called." << RESET << std::endl;
// }


// int	Socket::createConnection()
// {   
// 	socklen_t   addrlen = sizeof(_address);
// 	int		 new_socket; 
	
// 	//This function configures a socket to listen for incoming connection requests from clients. 
// 	//After binding a socket to an address and port, we use listen() to indicate that the socket is ready to accept incoming connections.

// 	std::cout << YELLOW << "--------- Waiting for new connection ----------" << RESET << std::endl;
// 	//This function is called to accept an incoming connection request on a socket that has been set up to listen for connections. 
// 	//When a client attempts to connect to the server, accept() creates a new socket for that connection and establishes the communication channel.
// 	new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
// 		if (new_socket >= 0)
// 		{
// 			std::cout << GREEN << "New connection accepted" << RESET << std::endl;
		
			
// 		inet_ntop(_address.ss_family, get_in_addr((struct sockaddr *)&_address), ip_address, sizeof(ip_address));
// 		std::cout << CYAN << "server: got connection from " << ip_address << RESET << std::endl;
// 		}
// 		else if (errno == EAGAIN || errno == EWOULDBLOCK)
// 		{
// 			// pollin_happened = manageConnection(new_socket);
// 			std::cout << CYAN << "No connections available, retrying..." << RESET << std::endl;
// 		}
// 		else
// 			std::cerr << RED << "Accept failed with error: " << strerror(errno) << RESET << std::endl;
// 	return (new_socket);
// }
