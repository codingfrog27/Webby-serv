/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_array.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschuijt <cschuijt@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/27 21:03:14 by cschuijt      #+#    #+#                 */
/*   Updated: 2023/03/27 21:03:14 by cschuijt      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// @brief Frees each element in a NULL-terminated array of pointers, then
// frees the array itself. Does not set array pointer to NULL.
void	free_array(void **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
