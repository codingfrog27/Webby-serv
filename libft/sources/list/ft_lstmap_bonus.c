/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/17 17:43:33 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/03/09 14:11:08 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*body;

	if (!lst || !f)
		return (NULL);
	head = ft_lstnew(f(lst->content));
	lst = lst->next;
	while (lst)
	{
		body = ft_lstnew(f(lst->content));
		if (!body)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, body);
		lst = lst->next;
	}
	return (head);
}
