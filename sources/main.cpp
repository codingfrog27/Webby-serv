/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: antoniosimone <antoniosimone@student.42      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/16 15:06:45 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/09/17 18:47:55 by antoniosimo   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	int server_fd; 
	int new_socket; //socket prepared to accept the connections
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	long valread;


	std::string http_header =	"HTTP/1.1 200 OK\r\n"
                            	"Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n";
								
	char file_path[1024] = "html/hello_message.html";
	
	//Create a socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cannot create socket");
		return (0);
	}

	//Identify (name) a socket
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;					//Fill the struct sockaddr_in 
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		return (0);
	}

	//On the server, wait for an incoming connection
	if (listen(server_fd, 3) < 0)
	{
		perror("In listen");
		exit (EXIT_FAILURE);
	}
	while (1)
	{
		std::cout << GREEN << "--------- Waiting for new connection ----------" << RESET << std::endl;
		std::cout << GREEN << "Hello from server" << RESET << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		
		//Send and receive messages
		char buffer[1024] = {0};

		valread = read(new_socket, buffer, 1024);
		if (valread < 0)
		{
			std::cout << "No bytes are there to read" << std::endl;
		}

		std::ifstream 	file(file_path);
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
		{
			perror("Could not open file!");
		}
		close(new_socket);
	}
	return 0;
}

//subject mentions poll() but can use select kque or epoll as well
