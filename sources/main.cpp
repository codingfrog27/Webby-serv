/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: antoniosimone <antoniosimone@student.42      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/06 17:47:46 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/10/21 16:33:03 by antoniosimo   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main()
{
	Config webserver;

	webserver.createSocketObject(webserver.parseConfigFile("example_config.conf"));
	// webserver.startParsing(config);

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



