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
        std::string _root;


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
        void setRoot(const std::string& root);
        void parseRule(const std::string &line);

        std::string getName() const;
};
        
bool	locationFound(std::string &line);
bool	checkCaracter(const std::string &line, const char &c);


