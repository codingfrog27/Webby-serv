/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   ft_strchr.c										:+:	:+:			*/
/*													 +:+					*/
/*   By: mde-cloe <mde-cloe@student.42.fr>			+#+					 */
/*												   +#+					  */
/*   Created: 2021/12/13 18:00:40 by mde-cloe	  #+#	#+#				 */
/*   Updated: 2022/03/09 14:10:04 by mde-cloe	  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char	*s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)s + i);
	return (NULL);
}
