/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Config.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/18 15:09:44 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/03 18:49:37 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once

#include <string>
#include <unordered_map>

class Config
{
	private:
		// Private Attributes
		

	public:
		// Constructors and Destructors
		Config(void);
		Config(const Config &rhs);
		Config &operator=(const Config &rhs);
		~Config(void);

	
	//these are all REQUIRED
	std::string	_serverPort;
	// size_t		_serverPort;
	std::string	_serverName;
	size_t		_maxConnects;
	size_t		_timeout; //general _timeout waiting for request/respond sending
	std::string	_rootDir;
	std::string	_autoIndexFilePath;
	int			_Timeout;
	// std::string	error_log_file;
	// std::string	access_log_file;

			
	// size_t		keep_alive_timeout; //spefically how long to keep an idle connection open before closing
	// bool		directory_listing; //aka autoindex: wether to display list of pages when requested dir isnt found
	// std::string	cgi_script_directory;
	// // not required but expected

	// size_t		max_keep_alive_requests;
	// std::unordered_map<std::string, std::string> custom_error_pages;
	// std::unordered_map<std::string, std::string> access_control;
};


