/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/13 18:31:29 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"
#include "Server.hpp"
#include "Config.hpp"


int	main(void)
{
	// std::cout << "Hello World!" << std::endl;
	int fd = open("/home/mde-cloe/PROJECTS/Webby-serv/chunked_request.txt", O_RDONLY);
	Request test(fd);
	test.readRequest();
	
	return (0);
}




// int main() {
// 	std::string fileName = "example_config.conf";
// 	std::vector<Config> configs = parseConfigFile("example_config.conf");

// 	try
// 	{
// 		if (!configs.empty()) {
// 			std::cout << configs[0] << std::endl;
// 			// configs[0].setServer(0);
// 			// std::cout << configs[0]._newLocations[0]->toString() << std::endl;
// 			// std::cout << configs[0]._newLocations[0]->_nestedLocations[0]->toString() << std::endl;
// 			// std::cout << configs[0]._newLocations[1]->toString() << std::endl;
// 		}
// 		 else {
// 			std::cout << "No configs found." << std::endl;
// 			configs.emplace_back();
// 		}
// 	Server	server(configs); //not constructable enough?
// 	server.main_server_loop();
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << "Exceotion caught!" << e.what() << std::endl;
// 	}
// 	return 0;
// }
