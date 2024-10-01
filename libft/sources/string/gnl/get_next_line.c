/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 19:19:31 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/08/02 20:30:52 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	buff[BUFFER_SIZE + 1];

	if (fd < 0 || read(fd, 0, 0) == -1 || BUFFER_SIZE < 1)
		return (NULL);
	line = stralloc(1);
	if (!line)
		return (NULL);
	if (*buff)
	{
		line = buff_to_line(buff, line);
		if (!line)
			return (NULL);
	}
	line = read_till_nl(buff, line, fd);
	if (!line)
		return (NULL);
	buff_update(buff);
	if (!*line)
		return (free(line), NULL);
	return (line);
}

char	*read_till_nl(char *buff, char *line, int fd)
{
	int	read_ret;

	read_ret = 1;
	while (!delimlen(line, '\n') && read_ret > 0)
	{
		read_ret = read(fd, buff, BUFFER_SIZE);
		if (read_ret == -1)
			return (NULL);
		buff[read_ret] = '\0';
		line = buff_to_line(buff, line);
		if (!line)
			return (NULL);
	}
	return (line);
}
