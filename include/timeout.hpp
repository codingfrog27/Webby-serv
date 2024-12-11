/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeout.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:10:11 by mde-cloe          #+#    #+#             */
/*   Updated: 2024/12/09 18:26:27 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <chrono>
#include <iostream>

typedef std::chrono::steady_clock::time_point t_time;
typedef std::chrono::milliseconds t_msecs;

t_time getStartTime();
bool isTimedOut(t_time startTime, t_msecs maxDuration);
t_msecs intToMsecs(int ms);
