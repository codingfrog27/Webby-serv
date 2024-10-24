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

class location
{
    private:
        // std::string _locationName;
        std::string _root;


    public:
        std::multimap<std::string, std::string> locationBlock;

        location(void);
        location(const location &rhs);
        location &operator=(const location &rhs);
        ~location(void);
        void	printLocationValue(const location& loc) const;
        std::string	getRoot(const location& loc);
        void	setRoot(const std::string& root);

};


