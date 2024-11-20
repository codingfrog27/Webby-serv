/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/20 16:46:45 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main() {
	std::string fileName = "example_config.conf";
	std::vector<std::unique_ptr<Config>> configs = parseConfigFile("example_config.conf");

	try
	{
		if (!configs.empty()) {
			for (size_t i = 0; i < configs.size(); i++){
				std::cout << "server block:" << configs[i]->toString() << "---------" << std::endl;
				for (size_t j = 0; j < configs[i]->_newLocations.size(); j++){
					std::cout << "location block:" << configs[i]->_newLocations[j]->toString() \
					<< "---------------" << std::endl;
				}
			}
			
			// // configs[0].setServer(0);
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
