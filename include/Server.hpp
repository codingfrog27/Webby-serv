/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/03 14:05:06 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/07 18:08:33 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Connection.hpp"
#include "Request.hpp"
#include "everything.hpp"


class Server
{
	private:
		std::vector<Socket> 	_serverSockets;
		std::vector<Config> 	&_serverBlocks;
		std::vector<pollfd>		_pollFDs;
		std::vector<Connection> _Connections;
		struct	addrinfo		*_addrInfo;
		// const int	_max_clients;

		void					setupAddrInfo(Config *config);
		void					acceptNewConnects(int i);
		void					close_connect(int i);
	public:
								Server(std::vector<Config>& vec);
								Server(Server &rhs) = delete;
								~Server(void);
		Server					&operator=(const Server &rhs) = delete;
		void					main_server_loop();
};
