/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientErrorExcept.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:19:12 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/08 19:04:22 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientErrorExcept.hpp"


// ************************************************************************** //
//                        Constructors and Destructors                        //
// ************************************************************************** //

ClientErrorExcept::ClientErrorExcept(unsigned int code, std::string msg) : _errorMsg(msg), _statusCode(code) {
}

ClientErrorExcept::ClientErrorExcept(const ClientErrorExcept &rhs)\
  : _errorMsg(rhs._errorMsg), _statusCode(rhs._statusCode)	{
}

ClientErrorExcept::~ClientErrorExcept(void){ //=default in header instead?
}


const char		*ClientErrorExcept::what() const noexcept
{
	return (_errorMsg.c_str());
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //


