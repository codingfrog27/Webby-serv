/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_lstadd_front_bonus.c							:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: asimone <asimone@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2021/12/14 22:10:05 by mde-cloe		  #+#	#+#			 */
/*   Updated: 2024/10/03 17:26:28 by asimone		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new_node)
{
	new_node->next = *lst;
	*lst = new_node;
}
