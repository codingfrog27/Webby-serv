/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 19:19:17 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/08/02 20:30:12 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "get_next_line.h"

char	*get_next_line(int fd);
char	*buff_to_line(char *buff, char *line);
size_t	delimlen(char *str, char delim);
void	buff_update(char *buff);
void	strcopycat(char *dst, char *cpysrc, char *catsrc, \
size_t catlen);
char	*stralloc(size_t space);
char	*read_till_nl(char *buff, char *line, int fd);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 500
# endif
#endif
