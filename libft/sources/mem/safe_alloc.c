/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   safe_alloc.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/20 19:44:48 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/03/20 19:44:48 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief protected malloc func to save on writing a check yourself everytime
 * assumes you're allowed to use printf and exit
 * @param size
 * @param count
 * @return ptr to allocated memory
 */
void	*safe_alloc(int size, int count)
{
	void	*heap_ptr;

	heap_ptr = ft_calloc((size_t)size, (size_t)count);
	if (!heap_ptr)
		error_exit("Malloc error", 1);
	return (heap_ptr);
}
