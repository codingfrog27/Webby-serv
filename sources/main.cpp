/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/23 16:24:14 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main()
{
	Config config;
	Config *webserver;
	

	webserver = config.parseConfigFile("example_config.conf");
	if (webserver)
	{
		// std::cout << "SERVER BLOCK BEFORE UPDATE: " << std::endl;
		// webserver->printBlockValue(config.serverBlock);
		// webserver->updateListenValue("listen", config);
		// std::cout << "SERVER BLOCK AFTER UPDATE: " << std::endl;
		// webserver->printBlockValue(webserver->serverBlock);
		// webserver->printBlockValue(webserver->locationBlock);
		
		delete webserver;
	}

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



