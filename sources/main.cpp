/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:47:46 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/06 13:06:38 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

// int main()
// {	
// 	std::ifstream file("example_config.conf");
// 	std::string buffer;

// 	std::vector<Config> servers = parseConfigFile("example_config.conf");
	
// 	return 0;
// }

int main() {
    std::string fileName = "example_config.conf";
    std::vector<Config> configs = parseConfigFile(fileName);

    if (!configs.empty()) {
        std::cout << configs[0].toString() << std::endl;
        std::cout << configs[0]._newLocations[0]->toString() << std::endl;
        std::cout << configs[0]._newLocations[0]->_nestedLocations[0].toString() << std::endl;
        std::cout << configs[0]._newLocations[1]->toString() << std::endl;
    } else {
        std::cout << "No configs found." << std::endl;
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



