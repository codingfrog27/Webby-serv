/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_map.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 20:15:22 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/10/14 20:19:36 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}
