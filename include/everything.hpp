/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   everything.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/02 17:11:40 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/03 15:21:58 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <unordered_map>
#include <sys/socket.h>

#include "Config.hpp"
#include "socket.hpp"
#include "Request.hpp"
#include "Connection.hpp"
#include "Server.hpp"
#include "Response.hpp"

enum Http_method
{
	INVALID = -1,
	GET,
	POST,
	DELETE,
	NOT_PARSED_YET,
};