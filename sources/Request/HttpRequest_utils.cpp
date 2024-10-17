/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   HttpRequest_utils.cpp							  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mde-cloe <mde-cloe@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/26 17:24:26 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/04 18:05:04 by mde-cloe		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Request.hpp"


std::string	Request::getHeaderValue(std::string key)
{
	std::unordered_map<std::string, std::string>::iterator it;
	
	it = _headers.find(key);
	if (it != _headers.end())
		return (it->second);
	return ("");
}



bool	Request::headerExists(std::string key)
{
	std::unordered_map<std::string, std::string>::iterator it;
	
	it = _headers.find(key);
	if (it != _headers.end())
		return (true);
	return (false);
}

std::string		Request::getStatusCode()
	{
		return (_statusCode);
	}

void	Request::printHeaders()
{

	for (const auto& pair : _headers) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
}