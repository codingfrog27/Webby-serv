/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/05 19:54:35 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "everything.hpp"
#include "Colors.hpp"

int main(int argc, char ** argv) {
	
	std::vector<Config> configs;
	try
	{
		parseConfig(argc, argv, configs);
		printConfigs(configs);
		Server _webbyserv(configs);
		_webbyserv.main_server_loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception caught!" << e.what() << std::endl;
	}
	return 0;
}


//aahhhhhhh i need break
//compile
//exception
//add missing closing brace errors

//getreqbody
//filepath string


//