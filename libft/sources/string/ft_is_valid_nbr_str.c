/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_is_valid_nbr_str.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/15 15:09:39 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/05/15 15:09:39 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//only checks on the first -, multiple should not be taken as valid input imo
// also doesnt allow extra space
bool	is_valid_nbr_str(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	if (str[i] == '-')
	{
		i++;
		if (!str[i])
			return (false);
	}
	while (str[i])
	{
		if (!ft_isdigit((int)str[i]))
			return (false);
		i++;
	}
	return (true);
}
