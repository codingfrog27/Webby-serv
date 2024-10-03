/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:05:06 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 15:37:39 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "everything.hpp"


class Server : public Config
{
	private:
		// Private Attributes
		

	public:
		const int	_max_clients = 50;
		// Constructors and Destructors

		Config *config;
		std::vector<Socket> _sockets;

		std::vector<pollfd>	pfds;
		std::vector<Connection>connections;

		// std::unordered_map<int, Connection> _connections;
	

		Server(Config *config); //socket init here
		Server(const Server &rhs);
		Server &operator=(const Server &rhs);
		~Server(void);

		// Public Methods
		void	main_server_loop();
		void	accept_loop();
		void	close_connect(Connection closeme, int i);
} ;


// 1 socket array, and then have always same size connections as pfds

// 2 connection map, based on 



// NEW PLAN

//1 make static arr of socket classes
// 2 accept loop and make pfd vec AND connection vec from it
// pfd and connect will have same index, connect will save socket FD for when writing back and closing