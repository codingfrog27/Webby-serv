/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   ft_split.c										 :+:	:+:			*/
/*													 +:+					*/
/*   By: mde-cloe <mde-cloe@student.42.fr>			+#+					 */
/*												   +#+					  */
/*   Created: 2021/12/13 18:00:40 by mde-cloe	  #+#	#+#				 */
/*   Updated: 2021/12/17 18:07:55 by mde-cloe	  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	wordcount;

	i = 0;
	wordcount = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			wordcount++;
		i++;
	}
	return (wordcount);
}

static size_t	wordlen(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] != c && s[len])
	{
		len++;
	}
	return (len);
}

static void	freetime(char **out, size_t current)
{
	size_t	i;

	i = 0;
	while (i < current)
	{
		free(out[i]);
		i++;
	}
	free(out);
}

char	**splittime(char const *s, char **out, char c, size_t wordcount)
{
	size_t	current;
	size_t	i;
	size_t	wlen;

	i = 0;
	while (s[i] == c && s[i])
		i++;
	current = 0;
	while (current < wordcount)
	{
		wlen = wordlen(s + i, c);
		out[current] = ft_substr(s, i, wlen);
		if (out[current] == NULL)
		{
			freetime(out, current);
			return (NULL);
		}
		i += wlen;
		while (s[i] == c && s[i])
			i++;
		current++;
	}
	out[current] = NULL;
	return (out);
}

char	**ft_split(char const *s, char c)
{
	char	**out;
	int		wordcount;

	if (!s)
		return (NULL);
	wordcount = count_words(s, c);
	out = (char **)malloc((wordcount + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	splittime(s, out, c, wordcount);
	return (out);
}
