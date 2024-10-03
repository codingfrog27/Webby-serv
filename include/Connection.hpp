/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:24:32 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 15:49:42 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "Response.hpp"
#include "HttpRequest.hpp"
#include "socket.hpp"
#include "Config.hpp"


class Connection : public HttpRequest
{
	private:
		// Private Attributes
		

	public:
		size_t		_socketIndex; //needed
		int			_serverFD;
		int			_clientFD;
		// HttpRequest	_request;
		// Response	*response;
		bool		_keepOpen;
		bool		doneReading;
		
		// Constructors and Destructors
		Connection(Config *config);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection(void);

		// Public Methods
		void	copy_clients_fds(Connection connects);
} ;
