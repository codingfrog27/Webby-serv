/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:09:44 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/09/18 15:36:05 by mde-cloe         ###   ########.fr       */
/*                                                                            */
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

	size_t		server_port;
	size_t		max_clients;
	size_t		timeout;
	std::string	document_root;
	std::string	default_index_file;
	std::string	error_log_file;
	std::string	access_log_file;
	//these are all REQUIRED

			
	std::string	server_name;
	size_t		keep_alive_timeout;
	bool		directory_listing;
	std::unordered_map<std::string, std::string> mime_types;
	std::string	cgi_script_directory;
	// not required but expected

	size_t		max_keep_alive_requests;
	std::string	ssl_certificate_file;
	std::string	ssl_certificate_key_file;
	std::unordered_map<std::string, std::string> custom_error_pages;
	std::unordered_map<std::string, std::string> access_control;
};

