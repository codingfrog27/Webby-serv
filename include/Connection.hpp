/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:24:32 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/02 17:03:54 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "Response.hpp"
#include "HttpRequest.hpp"
#include "socket.hpp"
#include "Config.hpp"


class Connection
{
	private:
		// Private Attributes
		

	public:
		Socket		_socket;
		int			_serverFD;
		int			_clientFD;
		HttpRequest	*_request;
		
		// Constructors and Destructors
		Connection(Config *config);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection(void);

		// Public Methods

} ;
