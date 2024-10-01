/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:14:24 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/01 17:51:37 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "Response.hpp"


#pragma once
class Client
{
	private:
		// Private Attributes
		

	public:
		// Constructors and Destructors


		HttpRequest		*req;
		Response		*response;
		bool			doneReading;
		bool			doneSending;
		bool			keep_open;
		
		int				status_code;
		int				FD;



		Client(void);
		Client(const Client &rhs);
		Client &operator=(const Client &rhs);
		~Client(void);

		// Public Methods

} ;
