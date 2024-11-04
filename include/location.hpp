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
        // void	setRoot(const location& loc);
        void parseRule(const std::string &line);

        std::string getName() const;

        std::string	getAlias();
        std::string	getAllowMethods();
        std::string	getAutoindex();
        std::string	getCgiExtension();
        std::string	getCgiPath();
        std::string	getReturn();
        std::string	getRoot();
        void	setAlias(const std::string &key);
        void	setAllowMethods(const std::string &key);
        void	setAutoindex(const std::string &key);
        void	setCgiExtension(const std::string &key);
        void	setCgiPath(const std::string &key);
        void	setReturn(const std::string &key);
        void	setRoot(const std::string &key);
        void    initializeLocation();

        std::string toString() const;
};
        
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);


