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

enum class connectStatus 
{
	SERV_SOCKET,
	IDLE,
	READING,
	REQ_ERR,
	CONNECT_CLOSED,
	DONE_READING,
	DONE_READING_CGI,
	RESPONDING,
	SERVER_ERR,
	FINISHED
};
class Connection
{
	private:
		// Private Attributes
		

	public:
		connectStatus	_CStatus;
		Config			*_config;
		Request			_request;
		bool			_isServerSocket;
		int				_clientFD;
		bool			_keepOpen;
		t_time			_startTime;
		t_msecs			_TimeoutTime;
		// bool		_doneReading;
		// Response	*response;
		
		// Constructors and Destructors
		Connection(Config *config, int clientFD, bool isServerside);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection();

		// Public Methods
} ;
