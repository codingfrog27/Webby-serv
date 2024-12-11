/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientErrorExcept.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:13:39 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/11 15:51:39 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>

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
