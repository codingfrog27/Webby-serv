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

Http_method Request::which_method_type(std::string str) 
{
	const char *Methods[] = {"GET", "POST", "DELETE"}; //will be updated after config parsing

	for (size_t i = 0; i < 3; i++)
	{
		if (str == Methods[i])
			return (static_cast<Http_method>(i));
	}
	throw std::invalid_argument("Unsupported HTTP method: " + str);
}

std::string Request::http_version(std::string version) //throw error if not 1 or 1.1
{
	if (!version.compare(0, 8, "HTTP/1.1"))
		return (version.substr(0, 8)); 
	else if (!version.compare(0, 6, "HTTP/1"))
		return (version.substr(0, 6));
	else
		throw std::invalid_argument("Unsupported HTTP version: " + version);
}

int	Request::convertChunkSize(const std::string &hexStr)
{
	size_t		chunkSize;
	char		*hexEnd;
	chunkSize = std::strtol(hexStr.data(), &hexEnd ,16); //alt: use parse_hex
	if (*hexEnd != '\r')
			throw ClientErrorExcept(400, "bad hex number in chunk");
	if (chunkSize + body_bytes_read > _max_body_size)
		throw ClientErrorExcept(413, "payload too large");

}