/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   str_switch.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/03 18:13:49 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/05/03 18:13:49 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	str_switch(char **str1, char **str2)
{
	char	*tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}
