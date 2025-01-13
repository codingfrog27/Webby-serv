/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeout.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:34:41 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/01/13 14:50:22 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timeout.hpp"

t_time getStartTime()
{
	return (std::chrono::steady_clock::now());
}

bool isTimedOut(t_time startTime, t_secs maxDuration)
{
	t_time now = std::chrono::steady_clock::now();
	t_secs timePassed =  std::chrono::duration_cast<std::chrono::\
							seconds>(now - startTime);
	if (timePassed > maxDuration)
		return (true);
	return (false);
}

t_secs intToMsecs(int ms)
{
	return (t_secs(ms));
}