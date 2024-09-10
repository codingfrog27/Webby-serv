/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_2d_charray.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/13 14:00:51 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/10/14 22:57:16 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_2d_charray(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		ft_printf("%s\n", map[i]);
		i++;
	}
}
