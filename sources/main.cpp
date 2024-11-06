/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/06 19:30:09 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main() {
	std::string fileName = "example_config.conf";
	std::vector<Config> configs = parseConfigFile("example_config.conf");

	try
	{
		if (!configs.empty()) {
			std::cout << configs[0] << std::endl;
			// configs[0].setServer(0);
			// std::cout << configs[0]._newLocations[0]->toString() << std::endl;
			// std::cout << configs[0]._newLocations[0]->_nestedLocations[0]->toString() << std::endl;
			// std::cout << configs[0]._newLocations[1]->toString() << std::endl;
		} else {
			std::cout << "No configs found." << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exceotion caught!" << e.what() << std::endl;
	}
	return 0;
}


//changes
// << operator instead of tostring
//moved ufncs to utils


// int	future_real_main(int argc, char **argv)
// {
// 	//if argv != 2 default config
// 	// else
// 	Config config(argv[1]);
// 	Connection *connections = server_setup(&config);
// 	main_server_loop();
// }
