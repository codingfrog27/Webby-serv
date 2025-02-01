/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientErrorExcept.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:13:39 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/02/01 15:51:58 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <iostream>

//put in request?
class ClientErrorExcept : std::exception
{
	private:

	public:
		std::string 	_errorMsg;
		unsigned int	_statusCode;
		
		// Constructors and Destructors
		ClientErrorExcept(unsigned int Code, std::string Msg);
		ClientErrorExcept() = delete;
		ClientErrorExcept(const ClientErrorExcept &rhs);
		ClientErrorExcept &operator=(const ClientErrorExcept &rhs) = delete;
		~ClientErrorExcept(void);
		const char		*what() const noexcept override;

		// Public Methods

} ;
