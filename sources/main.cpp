/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/06 19:15:55 by mde-cloe      #+#    #+#                 */
/*   Updated: 2025/04/17 14:17:19 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "everything.hpp"
#include "Colors.hpp"

int main(int argc, char ** argv) {
	
	std::vector<Config> configs;
	try
	{
		parseConfig(argc, argv, configs);
		// printConfigs(configs);
		Server _webbyserv(configs);
		_webbyserv.main_server_loop();
	}
	catch(const std::exception& e)
	{
		std::cout << "Exception caught!" << e.what() << std::endl;
	}
	return 0;
}