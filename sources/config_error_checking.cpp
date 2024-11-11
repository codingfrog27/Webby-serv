/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_error_checking.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:41:53 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/06 19:42:13 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::validateListen()
{
	std::string listen;
	if (!_rulemap.contains("listen"))
		throw std::invalid_argument("Error: listen directive not found");
	listen = _rulemap.at("listen");
	return (listen);
}