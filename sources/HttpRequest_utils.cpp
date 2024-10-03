/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest_utils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:24:26 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/03 18:20:31 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"


std::string	HttpRequest::getHeaderValue(std::string key)
{
	std::unordered_map<std::string, std::string>::iterator it;
	
	it = _headers.find(key);
	if (it != _headers.end())
		return (it->second);
	return ("");
}



bool	HttpRequest::headerExists(std::string key)
{
	std::unordered_map<std::string, std::string>::iterator it;
	
	it = _headers.find(key);
	if (it != _headers.end())
		return (true);
	return (false);
}

std::string		HttpRequest::getStatusCode()
	{
		return (_statusCode);
	}