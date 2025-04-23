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
#include "Response.hpp"
#include "socket.hpp"
#include "Config.hpp"

class Server;

enum class connectStatus 
{
	SERV_SOCKET,
	IDLE,
	READING,
	REQ_ERR,
	CONNECT_CLOSED,
	DONE_READING,
	CGI_REQUIRED,
	CGI,
	RESPONDING,
	SERVER_ERR,
	FINISHED
};
class Connection
{
	private:
		// Private Attributes
		

	public:
		connectStatus					_CStatus;
		Config							*_config;
		Request							_request;
		Response						_response;
		std::shared_ptr<CGI>			_cgi;
		bool							_isClientSocket;
		bool							_wantsNewConnect;
		int								_clientFD;
		bool							_keepAlive;
		t_time							_startTime;
		t_secs							_IdleTimeout;
		// bool							_doneReading;
		
		// Constructors and Destructors
		Connection(Config *config, int clientFD, bool isServerside);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection();

		// Public Methods
		// void	resetRequest(Config* config, int clientFD);
		// void	resetResponse();
		connectStatus	refreshIfKeepAlive();
		void			connectionAction(const pollfd &poll, Server &server);
		void			findFDtoRemove(int eraseMe, std::vector<pollfd> &pollFDs);
		void			removeCGIFromEverywhere(Server& server);
		connectStatus	checkConnectStatus(const pollfd &poll);
		static bool		connectIsOkay(int fd);
} ;
