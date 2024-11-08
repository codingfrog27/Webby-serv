/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionClosedExcep.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:14:55 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/11/08 19:21:41 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include "Colors.hpp"

#pragma once

class ConnectionClosedExcep : public std::exception
{
	private:
		// Private Attributes
		

	public:
		int	_clientFD;
		// Constructors and Destructors
		ConnectionClosedExcep(int clientFD);
		ConnectionClosedExcep() = delete;
		ConnectionClosedExcep(const ConnectionClosedExcep &rhs);
		ConnectionClosedExcep &operator=(const ConnectionClosedExcep &rhs) = delete;
		~ConnectionClosedExcep(void);
		const char		*what() const noexcept override;

		// Public Methods

} ;
