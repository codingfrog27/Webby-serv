/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/19 19:45:25 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main() {
	std::string fileName = "example_config.conf";
	std::vector<Config> configs = parseConfigFile("example_config.conf");

	try
	{
		if (!configs.empty()) {
			std::cout << configs[0].toString() << std::endl;
			// // configs[0].setServer(0);
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
