/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asimone <asimone@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:06:45 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/19 16:10:08 by asimone          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;

	std::string file_path = "html/hello_message.html";

	Socket test_server = Socket("127.0.0.1", "8001");
	test_server.createConnection(file_path);
	
	return (0);
}

//subject mentions poll() but can use select kque or epoll as well