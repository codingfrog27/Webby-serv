/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:13:28 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/06 17:15:21 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
void telnet_send_receive_loop(int client_fd);

//system is little endian so need to change address and IP

uint16_t manual_htons(uint16_t hostshort) {
	// Check if the host is little-endian or big-endian
	uint16_t test = 1;
	bool is_little_endian = *((uint8_t*)&test) == 1;

	if (is_little_endian) {
	// Convert to big-endian (network byte order)
	return (hostshort >> 8) | (hostshort << 8);
	} else {
	// Host is already big-endian, no need to convert
	return hostshort;
	}
}

uint32_t manual_htonl(uint32_t hostlong) {
	uint16_t test = 1;
	bool is_little_endian = *((uint8_t*)&test) == 1;

	if (is_little_endian) {
	return ((hostlong >> 24) & 0x000000FF) |
	   ((hostlong >> 8) & 0x0000FF00) |
	   ((hostlong << 8) & 0x00FF0000) |
	   ((hostlong << 24) & 0xFF000000);
	} else {
	return hostlong;  // Already big-endian
	}
}


//testing sockets
int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	Telnet_server();
	return 0;
}




int	Telnet_server()
{

	int server_fd = socket(AF_INET, SOCK_STREAM, 0); //making socket
	if (server_fd == -1)
		return (std::cerr << "Failed to create socket!" << std::endl, -1);

	sockaddr_in	server_addr; //contains IP and port
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = manual_htons(8080);  // converted to little endian
	server_addr.sin_addr.s_addr = INADDR_ANY; //any interface
	if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) //binded
		return (std::cerr << "Failed to bind socket!" << std::endl, -1);

	if (listen(server_fd, 5) == -1) 	// listen for connections
		return (std::cerr << "Failed to listen!" << std::endl, -1);
   	std::cout << "Listening for connections on port 8080..." << std::endl;


	sockaddr_in client_addr;
	socklen_t client_size = sizeof(client_addr);
	int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_size); //accept connection (blocks here)

	if (client_fd == -1)
		return (std::cerr << "Failed to accept client connection!" << std::endl, -1);

	const char *welcome_msg = "Welcome to the Telnet Server!\r\n";
	send(client_fd, welcome_msg, strlen(welcome_msg), 0);
	telnet_send_receive_loop(client_fd);
	close(client_fd);
	close(server_fd);
}

void telnet_send_receive_loop(int client_fd)
{
	char buffer[256];

	while (true) 
	{
		memset(buffer, 0, 256);
		int bytes_received = recv(client_fd, buffer, 256, 0);
		if (bytes_received <= 0)
		{
			std::cout << "Client disconnected." << std::endl;
			break;
		}
		send(client_fd, "msg received :)\n you send to me: ", 34, 0); // Echo the received message back to the client
		send(client_fd, buffer, bytes_received, 0);
	}
}

//subject mentions poll() but can use select kque or epoll as well
