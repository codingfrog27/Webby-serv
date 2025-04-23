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
		return (_statusStr);
	}

void	Request::printHeaders()
{

	for (const auto& pair : _headers) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
}

Http_method Request::which_method_type(std::string str)
{
	const char *Methods[] = {"GET", "POST", "DELETE"};
	for (int i = 0; i < 3; i++)
	{
		if (str == Methods[i])
		{
			_method_type = static_cast<Http_method>(i);
			for (auto allowed : _allowedMethods)
			{
				if (allowed == _method_type)
					return (_method_type);
			}
			throw ClientErrorExcept(405, "405 Method Not Allowed");
		}
	}
	throw ClientErrorExcept(400, "400 Unsupported HTTP method: " + str);
}

std::string Request::http_version(const std::string &version)
{
	if (!version.compare(0, 8, "HTTP/1.1"))
		return (version.substr(0, 8));
	else if (!version.compare(0, 6, "HTTP/1"))
		return (version.substr(0, 6));
	else
		throw ClientErrorExcept(400, "400 Unsupported HTTP version: " + version);
}

int	Request::convertChunkSize(const std::string &hexStr, size_t &hexStrSize)
{
	size_t		chunkSize;
	char		*hexEnd;
	chunkSize = std::strtol(hexStr.data(), &hexEnd ,16);
	std::cout << "here :" << chunkSize << *hexEnd << std::endl;
	if (*hexEnd != '\r' && chunkSize != 0)
			throw ClientErrorExcept(400, "400 bad hex number in chunk");
	if (chunkSize + body_bytes_read > _max_body_size)
		throw ClientErrorExcept(413, "413 payload too large");
	hexStrSize = (hexEnd - hexStr.c_str());
	return (chunkSize);
}

const std::string &Request::getBody()
{
	return (_reqBody);
}

std::string trim(const std::string& str) {
	auto start = str.find_first_not_of(" \n\r\t");
	auto end = str.find_last_not_of(" \n\r\t");
	return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}
