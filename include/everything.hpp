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

#include "socket.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Connection.hpp"
