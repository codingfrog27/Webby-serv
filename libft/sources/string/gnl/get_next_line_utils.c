/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 19:19:25 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/08/02 20:44:36 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*buff_to_line(char *buff, char *line)
{
	size_t	line_len;
	size_t	buff_end;
	char	*line2;

	line_len = delimlen(line, '\0');
	buff_end = delimlen(buff, '\n');
	if (!buff_end)
		buff_end = delimlen(buff, '\0');
	line2 = stralloc(buff_end + line_len + 1);
	if (!line2)
		return (NULL);
	strcopycat(line2, line, buff, buff_end);
	free(line);
	return (line2);
}

void	buff_update(char *buff)
{
	size_t	i;
	size_t	nl_len;
	size_t	bufflen;

	i = 0;
	nl_len = delimlen(buff, '\n');
	bufflen = delimlen(buff, '\0');
	if (buff[nl_len] == '\0')
	{
		while (i < BUFFER_SIZE)
		{
			buff[i] = '\0';
			i++;
		}
		return ;
	}
	while (i <= bufflen - nl_len)
	{
		buff[i] = buff[nl_len + i];
		i++;
	}
	buff[i] = '\0';
}

//wish functions could take 5 args, then i wouldnt need to strlen again

void	strcopycat(char *dst, char *cpysrc, char *catsrc, \
size_t catlen)
{
	size_t	cpylen;
	size_t	i;
	size_t	j;

	cpylen = delimlen(cpysrc, '\0');
	i = 0;
	j = 0;
	while (i < cpylen)
	{
		dst[i] = cpysrc[i];
		i++;
	}
	while (j < catlen)
	{
		dst[i + j] = catsrc[j];
		j++;
	}
}

/*
	 Will return len+1 for newlines (and any delim besides \0)
	 since the newline should be included in the file
*/
size_t	delimlen(char *str, char delim)
{
	size_t	i;

	i = 0;
	if (delim != '\0')
	{
		while (str[i])
		{
			if (str[i] == delim)
				return (i + 1);
			i++;
		}
		return (0);
	}
	while (str[i])
		i++;
	return (i);
}

char	*stralloc(size_t space)
{
	char	*str;
	size_t	i;

	str = malloc(space * sizeof(char));
	i = 0;
	if (!str)
		return (NULL);
	while (i < space)
	{
		str[i] = '\0';
		i++;
	}
	return (str);
}
