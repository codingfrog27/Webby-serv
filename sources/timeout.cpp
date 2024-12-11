/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeout.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:34:41 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/10 18:51:01 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timeout.hpp"

t_time getStartTime()
{
	return (std::chrono::steady_clock::now());
}

bool isTimedOut(t_time startTime, t_msecs maxDuration)
{
	t_time now = std::chrono::steady_clock::now();
	t_msecs timePassed =  std::chrono::duration_cast<std::chrono::\
							milliseconds>(now - startTime);
	if (timePassed > maxDuration)
		return (true);
	return (false);
}

t_msecs intToMsecs(int ms)
{
	return (t_msecs(ms));
}