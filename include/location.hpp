/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.hpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/05 14:22:47 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/09/05 15:38:48 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

enum class Http_method;

class location
{
	private:
		std::string	findLocationName(std::string locationLine);
		std::string	validateLocationName(std::string line);


	public:
		std::string				 	_alias;
		std::vector<Http_method>	_allow_methods;
		bool						_autoindex;
		std::vector<std::string>	_cgi_extension;
		std::vector<std::string>	_cgi_path; 
		std::vector<std::string>	_index;
		std::string					 _return;
		std::string					 _root;
		std::string 				_name;

		std::vector<location>   _nestedLocations;
		std::unordered_map<std::string, std::string> _rulemap;

		location(void);
		location(std::ifstream &file, std::string &line);
		location(const location &rhs);
		location &operator=(const location &rhs);
		~location(void);
		
		void	printLocationValue(const location& loc) const;
		void parseRule(const std::string &line);

		std::string &getName();

		std::string	getAlias();
		std::vector<Http_method>	getAllowMethods();
		bool		getAutoindex();
		std::vector<std::string>	getCgiExtension();
		std::vector<std::string>	getCgiPath();
		std::vector<std::string>	getIndex();
		std::string	getReturn();
		std::string	getRoot();
		void	setAlias(const std::string &alias);
		void	setAllowMethods(const std::vector<Http_method> &allow_methods);
		void	setAutoindex(const bool &autoindex);
		void	setCgiExtension(const std::vector<std::string> &cgi_extension);
		void	setCgiPath(const std::vector<std::string> &cgi_path);
		void	setIndex(const std::vector<std::string> &index);
		void	setReturn(const std::string &t_return);
		void	setRoot(const std::string &root);
		void	setName(const std::string &name);

		void	initializeLocation();

		std::string toString() const;
		std::string validateAlias();
		std::vector<Http_method> validateAllowMethods();
		bool validateAutoindex();
		std::vector<std::string> validateIndex();
		std::string validateReturn();
		std::string validateRoot();

		std::vector<std::string> validateCgiExt();
		std::vector<std::string> validateCgiPath();
};
		
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);
std::string	normalize_space_location(std::string& str);

