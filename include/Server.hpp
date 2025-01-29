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

class CGI;

class Server
{
	private:
		std::vector<Socket> 	_serverSockets;
		std::vector<Config> 	&_serverBlocks;
		std::vector<pollfd>		_pollFDs;
		std::vector<pollfd>		_CGIPollFDs;
		// std::vector<Connection> _Connections;
		std::unordered_map<int, Connection> _Connections;
		std::unordered_map<int, std::shared_ptr<CGI>> _CGIs;
		struct	addrinfo		*_addrInfo;
		// const int	_max_clients;

		void		setupAddrInfo(Config *config);
		void		handleCGIPollEvents();
		void		acceptNewConnects(size_t size);
		void		close_connect(int i);
		void		closeCGIConnects();
	public:
					Server(std::vector<Config>& vec);
					Server(Server &rhs) = delete;
					~Server(void);
		Server		&operator=(const Server &rhs) = delete;
		void		main_server_loop();
		void		connectionAction(Connection &connect, pollfd &poll);
		void		PrintConnectionStatusses(size_t size);
};