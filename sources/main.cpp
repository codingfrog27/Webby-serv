/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 15:50:16 by mde-cloe         ###   ########.fr       */
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

void	Connection::copy_clients_fds(Connection connects)
{
	
}



