/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/13 18:00:40 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/10/13 17:11:54 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief
 * copies 2 strings and copies them into a newly allocated third string
 * new string is alloced on HEAP and source strings are assumed to be on STACK
 * SO DONT FORGET TO FREE OLD STRINGS IF THEYRE ON HEAP TO PREVENT LEAKS
 * @param s1
 * @param s2
 * @return the new string
 */
char	*ft_strjoin(char const	*s1, char const	*s2)
{
	char	*s3;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	s3 = malloc(len * sizeof(char));
	if (!s3)
		return (NULL);
	ft_strlcpy(s3, s1, len);
	ft_strlcat(s3, s2, len);
	return (s3);
}

void	ez_strcpy(char *dest, char *src, int start, int len)
{
	int	i;

	i = 0;
	while (i < len && src[i])
	{
		dest[start] = src[i];
		i++;
		start++;
	}
}

char	*protected_str_iple_join(char *s1, char *s2, char *s3)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;
	char	*ret;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s3)
		s3_len = ft_strlen(s3);
	else
		s3_len = 0;
	ret = safe_alloc(s1_len + s2_len + s3_len + 1, sizeof(char));
	ez_strcpy(ret, s1, 0, s1_len);
	ez_strcpy(ret, s2, s1_len, s2_len);
	if (s3_len)
		ft_strlcat(ret, s3, s1_len + s2_len + s3_len + 1);
	return (ret);
}
