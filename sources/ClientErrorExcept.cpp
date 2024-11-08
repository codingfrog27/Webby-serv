/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientErrorExcept.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:19:12 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/08 15:31:17 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientErrorExcept.cpp"


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


const char		*what() const noexcept override
{
	return ((std::to_string(_statusCode) + " " + _errorMsg).c_str());
}

// ************************************************************************** //
//                                Public methods                              //
// ************************************************************************** //


