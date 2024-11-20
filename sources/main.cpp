/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/20 17:42:08 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"
bool	noDoublePorts(const std::vector<std::unique_ptr<Config>> &configs);
#include "Colors.hpp"

int main() {
	std::string fileName = "example_config.conf";
	std::vector<std::unique_ptr<Config>> configs = parseConfigFile("example_config.conf");

	try
	{
		if (!configs.empty()) {
			for (size_t i = 0; i < configs.size(); i++){
				std::cout << "server block:\n" << configs[i]->toString() << "---------" << std::endl;
				for (size_t j = 0; j < configs[i]->_newLocations.size(); j++){
					std::cout << "location block:\n" << configs[i]->_newLocations[j]->toString() \
					<< "---------------" << std::endl;
					for (size_t k = 0; k < configs[i]->_newLocations[j]->_nestedLocations.size(); k++)
					{
						std::cout << "NESTED location block:\n" << configs[i]->_newLocations[j]->_nestedLocations[0]->toString()\
						 << std::endl;
					}
				}
			}
		if (noDoublePorts(configs))
			std::cout << "Ciao" << std::endl;
		else
			std::cout << ":(((())))" << std::endl;
			
			// // configs[0].setServer(0);
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

bool	noDoublePorts(const std::vector<std::unique_ptr<Config>> &configs)
{
	for (size_t i = 0; i < configs.size(); i++)
	{
		const std::string &port = configs[i]->getListen();
		for (size_t j = i + 1; j < configs.size(); j++)
		{
			if (port == configs[j]->getListen())
				return (false);
		}
	}
	return (true);
}
