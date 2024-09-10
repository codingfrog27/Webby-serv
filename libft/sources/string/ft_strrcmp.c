/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrcmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/20 20:34:20 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/10/12 16:29:15 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
	* @brief function that sees if 2 strings are the same, counting backwards.
	* @param n amount of bytes to check
	* @return [true] If strings are the same for n chars else [false].
*/
bool	ft_strrcmp(char *str1, char *str2, size_t n)
{
	long	i;
	long	j;

	if (!str1 || !str2 || !n)
		return (false);
	i = ft_strlen(str1);
	j = ft_strlen(str2);
	while (i >= 0 && j >= 0 && str1[i] == str2[j])
	{
		i--;
		j--;
		n--;
		if (n < 1)
			return (true);
	}
	return (false);
}
