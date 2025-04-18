/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   ft_substr.c										:+:	:+:			*/
/*													 +:+					*/
/*   By: mde-cloe <mde-cloe@student.42.fr>			+#+					 */
/*												   +#+					  */
/*   Created: 2021/12/13 18:00:40 by mde-cloe	  #+#	#+#				 */
/*   Updated: 2021/12/13 18:12:21 by mde-cloe	  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief allocates a smaller string from a bigger one
 * 
 * @param s string to grab a part from
 * @param start the start of s that you want 
 * @param len length of your substr
 * @return char* opinter to the new string
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if ((size_t)start >= slen || !len)
		return (ft_strdup(""));
	if (len > slen)
		len = slen;
	substr = ft_calloc(len + 1, sizeof(char));
	if (!substr)
		return (NULL);
	ft_memcpy(substr, s + start, len);
	return (substr);
}
