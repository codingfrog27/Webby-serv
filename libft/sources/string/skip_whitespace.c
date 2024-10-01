/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   skip_whitespace.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/15 11:27:21 by mde-cloe      #+#    #+#                 */
/*   Updated: 2024/02/15 11:27:21 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * @brief returns amount to add to your index to jump over a strings space chars
 *
 * @param str
 * @param i
 * @return int
 */
int	skip_space(char *str, int i)
{
	int	ret;

	ret = 0;
	if (!str || !str[i])
		return (0);
	while (str[i + ret] == ' ')
		ret++;
	return (ret);
}

int	skip_whitespace(char *str, int i)
{
	int	ret;

	ret = 0;
	if (!str)
		return (0);
	if (!str || !str[i])
		return (0);
	while (str[i + ret] && ft_strchr("\t\n\v\f\r ", str[i + ret]))
		ret++;
	return (ret);
}
