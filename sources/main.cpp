/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/30 11:59:04 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main()
{
	Config config;
	location loc;
	

	std::vector<Config> servers = parseConfigFile("example_config.conf");
	// std::cout << "SERVER BLOCK BEFORE UPDATE: " << std::endl;
	// webserver->printBlockValue(config.serverBlock);
	// webserver->updateListenValue("listen", config);
	// std::cout << "SERVER BLOCK AFTER UPDATE: " << std::endl;
	// config.printBlockValue(config.serverBlock);
	// webserver->printBlockValue(webserver->locationBlock);
	// loc.setRoot(loc.getRoot(config.findLocation(config._locations, "/tours")));
	// config.printBlockValue(config._locations.find("/tours")->second.locationBlock);
	// loc.getRoot(loc);

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



