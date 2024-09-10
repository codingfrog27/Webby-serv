/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ptrarr_len.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/20 18:40:13 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/03/20 18:40:13 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief counts the amount of ptrs in your ptr array, assumes NULL termination
 *
 * @param arr
 * @return amount of pointers
 */
int	ptrarr_len(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
