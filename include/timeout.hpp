/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeout.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-cloe <mde-cloe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:10:11 by mde-cloe          #+#    #+#             */
/*   Updated: 2025/01/14 13:09:40 by mde-cloe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <chrono>
#include <iostream>

typedef std::chrono::steady_clock::time_point t_time;
typedef std::chrono::seconds t_secs;

t_time getStartTime();
bool isTimedOut(t_time startTime, t_secs maxDuration);
t_secs setTimeout(int ms);
