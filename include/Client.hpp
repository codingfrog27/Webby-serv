/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:14:24 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/10/01 16:00:59 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "Http_response.hpp"


#pragma once
class Client
{
	private:
		// Private Attributes
		

	public:
		// Constructors and Destructors


		HttpRequest		*req;
		Http_response	*response;
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
