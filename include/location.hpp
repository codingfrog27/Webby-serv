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

class location
{
    private:
        // std::string _locationName;


    public:
        std::string                 _alias;
        std::vector<std::string>    _allow_methods;
        bool                        _autoindex;
        std::vector<std::string>    _cgi_extension;
        std::vector<std::string>    _cgi_path; 
        std::string                 _index;
        std::string                 _return;
        std::string                 _root;

        std::vector<std::unique_ptr<location>> _nestedLocations;
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
        std::vector<std::string>	getAllowMethods();
        bool	    getAutoindex();
        std::vector<std::string>	getCgiExtension();
        std::vector<std::string>    getCgiPath();
        std::string	getIndex();
        std::string	getReturn();
        std::string	getRoot();
        void	setAlias(const std::string &alias);
        void	setAllowMethods(const std::vector<std::string> &allow_methods);
        void	setAutoindex(const bool &autoindex);
        void	setCgiExtension(const std::vector<std::string> &cgi_extension);
        void	setCgiPath(const std::vector<std::string> &cgi_path);
        void	setIndex(const std::string &index);
        void	setReturn(const std::string &return);
        void	setRoot(const std::string &root);
        void    initializeLocation();

        std::string toString() const;
        std::string validateAlias();
        bool validateAutoindex();
        std::string validateReturn();
        std::string validateRoot();

        std::vector<std::string> validateCgiExt();
        std::vector<std::string> validateCgiPath();
};
        
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);


