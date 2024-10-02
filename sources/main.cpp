/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/02 17:40:50 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main()
{
	int test_fd = open("test.txt", 0);
	if (test_fd <= 0)
		std::cerr << "open failed" << std::endl;
	HttpRequest request(test_fd);
	return 0;
}

Connection *server_setup(Config *config)
{
	// Connection* hoi = new (Connection(config))[config->max_clients];
	std::vector<Connection> connections(config->max_clients);


	//bruhh syntatx


	// 1 make vec or array of connections
	

	// delete[] hoi;
}


int	future_real_main(int argc, char **argv)
{
	//if argv != 2 default config
	// else
		Config config(argv[1]);
	Connection *connections = server_setup(&config);
	main_server_loop();
}

std::vector<pollfd>	copy_clients_fds(Connection *connects)
{
	
}

void	main_server_loop(Connection *connections)
{
	int num_events;
	std::vector<pollfd>	pfds;
	while (1)
		{
			pfds = copy_clients_fds(connections);
			// check for new connects, if yes expand our vectors
			num_events = poll(pfds.data(), pfds.size(), 2500);
			if (num_events < 0) 
			{
				std::cerr << RED << "Poll failed with error: " << strerror(errno) << RESET << std::endl;
				break;
			}
			if (num_events == 0) 
				std::cout << MAGENTA << "Poll timed out, no events to handle." << RESET << std::endl;
			for (size_t i = 0; i < pfds.size(); ++i)
			{
				if (pfds[i].revents & POLLIN) 
					Connection[i].req->main_reader(pfds[i].fd);
				if ((pfds[i].revents & POLLOUT) && Connection[i].doneReading) //what if ready to post to server but not reday for response
					Connection[i].response->generateResponse();
				if (!Clients[i].keep_open)
					// close connection and reopen new one
			}
			
			/* code */
		}
}