/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/29 16:34:43 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"
std::vector<Config>	parseConfigFile(const std::string fileName);


int main()
{
	Config config;
	location loc;
	// std::vector<Config> servers
	

	std::vector<Config> servers = parseConfigFile("example_config.conf");



	for (const auto& it : servers)
	{
		for (const auto& pair : it._rulemap) {
        	std::cout << pair.first << ": " << pair.second << std::endl;
		}
    }

	
	// Server(servers);


	// std::cout << "SERVER BLOCK BEFORE UPDATE: " << std::endl;
	// webserver->printBlockValue(config.serverBlock);
	// webserver->updateListenValue("listen", config);
	// std::cout << "SERVER BLOCK AFTER UPDATE: " << std::endl;
	// config.printBlockValue(config.serverBlock);
	// webserver->printBlockValue(webserver->locationBlock);
	// loc.setRoot(loc.getRoot(config.findLocation(config._locations, "/tours")));
	// config.printBlockValue(config._locations.find("/tours")->second.locationBlock);

	return 0;
}


// int	future_real_main(int argc, char **argv)
// {
// 	//if argv != 2 default config
// 	// else
// 	Config config(argv[1]);
// 	Connection *connections = server_setup(&config);
// 	main_server_loop();
// }



