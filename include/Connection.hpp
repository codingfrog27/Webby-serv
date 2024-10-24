/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Connection.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/02 16:24:32 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/04 17:58:36 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once


#include "Request.hpp"
#include "socket.hpp"
#include "Config.hpp"

// class Request;

class Connection
{
	private:
		// Private Attributes
		

	public:
		bool		_isServerSocket;
		int			_clientFD;
		Request		_request;
		Config		*_config;
		bool		_keepOpen;
		// bool		_doneReading;
		// Response	*response;
		
		// Constructors and Destructors
		Connection(Config *config, int clientFD, bool isServerside);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection();

		// Public Methods
} ;
