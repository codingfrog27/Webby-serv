/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstsize_bonus.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/14 22:18:54 by mde-cloe      #+#    #+#                 */
/*   Updated: 2021/12/15 16:40:06 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_lstsize(t_list *lst)
// {
// 	int	i;

// 	i = 0;
// 	while (lst)
// 	{
// 		i++;
// 		lst = lst->next;
// 	}
// 	return (i);
// }

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}
