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
		// Private Attributes
		void	setupAddrInfo(Config *config);

	public:
		// const int	_max_clients;
		// Constructors and Destructors


		// Config *_config;
		std::vector<Socket> _serverSockets;
		std::vector<Config> _serverBlocks;

		std::vector<pollfd>		_pollFDs;
		std::vector<Connection> _Connections;
		struct	addrinfo		*_addrInfo;

		// std::unordered_map<int, Connection> _connections;
	

		Server(const std::vector<Config>& vec); //socket init here
		// Server(const Server &rhs);
		// Server &operator=(const Server &rhs);
		~Server(void);

	// Public Methods
	void		main_server_loop();
	void		acceptNewConnects(int i);
	void		close_connect(int i);
} ;


// 1 socket array, and then have always same size connections as pfds

// 2 connection map, based on 



// NEW PLAN

//1 make static arr of socket classes
// 2 accept loop and make pfd vec AND connection vec from it
// pfd and connect will have same index, connect will save socket FD for when writing back and closing