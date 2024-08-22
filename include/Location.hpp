/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 18:41:56 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/08/22 19:04:02 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>

#define CYAN	"\033[36m"
#define GREEN	"\033[32m"
#define MAGENTA "\033[35m"
#define RED     "\033[31m"
#define YELLOW	"\033[33m"
#define RESET	"\033[0m"

class Location 
{
private:
    bool						m_autoIndex; //Automatic directory indexing is enabled for the location
    std::string 				m_alias; //Alternative physical path that maps to the location. It allows you to define a different directory that corresponds to the URL path
	std::string 				m_cgi; //Path to the CGI (Common Gateway Interface) binary or script that should be executed for requests to this location
    std::map<int, std::string>	m_errorPages; //HTTP status codes to custom error pages
    std::vector<std::string>	m_indexFiles; //List of index file names to be used when a directory is requested
	std::set<std::string>		m_methods; //The HTTP methods (such as GET, POST, PUT, DELETE) that are allowed or supported for the location
	std::string					m_path; //URL path for the location
    std::string 				m_redirect; //URL to which requests should be redirected

public:
												//ORTHODOX CANONICAL CLASS FORM//
    Location();
    Location(const Location &obj);
    Location& operator=(const Location& obj);
    ~Location();

															//GET FUNCTIONS
	const	std::string &getAlias();
	const	bool &getAutoIndex();
	const	std::string &getCgi();
	const	std::map<int, std::string> &getErrorPages();
	const	std::vector<std::string> &getIndexFiles();
	const	std::set<std::string> &getMethods();
	const 	std::string &getPath();
	const	std::string &getRedirect();

															//SET FUNCTIONS
	void setAlias(const bool &t_alias);;
	void setAutoIndex(const bool &t_autoIndex);
	void setCGI(const std::string &t_cgi);
	void setErrorPages(const std::map<int, std::string> &t_errorPages);
	void setIndexFiles(const std::vector<std::string> &t_indexFiles);
	void setMethods(const std::set<std::string> &t_methods);
	void setPath(const std::string &t_path);
	void setRedirect(const std::string &t_redirect);
};