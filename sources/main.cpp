/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:15:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/11 16:36:44 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"

int main() {
	std::string fileName = "example_config.conf";
	std::vector<Config> configs = parseConfigFile("example_config.conf");

	try
	{
		if (!configs.empty()) {
			// std::cout << configs[0].toString() << std::endl;
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


//changes
// << operator instead of tostring
//moved ufncs to utils

//think we can use strings right away, problem was calling setserver afterwards in main loop

//i dont think we have to check for existence in the setters (likei in setmaxbody size)
// since it will only go in there if already found
// we should however check for errors when setting each member


// and after setting everything look for unset values
//alternatively instead of looping through the whole map we can call find each time and do the
// check for empty feels there directly, but since we alreayd have the loop lets use it :)



//she works!! I think actually we can even move the contains if statements into the set
// functions themselves :) it never needed to be a for loop cause contains just finds it directly ;)








// int	future_real_main(int argc, char **argv)
// {
// 	//if argv != 2 default config
// 	// else
// 	Config config(argv[1]);
// 	Connection *connections = server_setup(&config);
// 	main_server_loop();
// }
