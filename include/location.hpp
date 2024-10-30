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
#include <unordered_map>
#include <vector>

class location
{
    private:
        // std::string _locationName;
        std::string     _alias;
        std::string     _allow_methods;
        std::string     _autoindex;
        std::string     _cgi_extension;
        std::string     _cgi_path; 
        std::string     _index;
        std::string     _return;
        std::string     _root;


    public:
        std::vector<location> _nestedLocations;
        std::unordered_map<std::string, std::string> _locationBlock;
        std::unordered_map<std::string, std::string> _rulemap;
        std::string _name;

        location(void);
        location(std::ifstream &file, std::string &line);
        location(const location &rhs);
        location &operator=(const location &rhs);
        ~location(void);
        void	printLocationValue(const location& loc) const;
        std::string	getRoot(const location& loc);
        // void	setRoot(const location& loc);
        void parseRule(const std::string &line);

        std::string getName() const;
        std::string	location::getAlias();
        void	location::setAlias(const std::string &value);
        std::string	location::getAllowMethods();
        void	location::setAllowMethods(const std::string &value);
        std::string	location::getAutoindex();
        void	location::setAutoindex(const std::string &value);
        std::string	location::getCgiExtension();
        void	location::setCgiExtension(const std::string &value);
        std::string	location::getCgiPath();
        void	location::setCgiPath(const std::string &value);
        std::string	location::getReturn();
        void	location::setReturn(const std::string &value);
        std::string	location::getRoot();
        void	location::setRoot(const std::string &value);
};
        
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);


