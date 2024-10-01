/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_exit.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/20 19:57:01 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/03/20 19:57:01 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "h_colors.h"

/**
 * @brief prints an error on stderr and exits
 *
 * @param str the msg
 * @param err_nbr the error code to exit with
 */
void	error_exit(char *str, int err_nbr)
{
	write(2, C_RED, ft_strlen(C_RED));
	write(2, str, ft_strlen(str));
	write(2, C_RESET, ft_strlen(C_RESET));
	exit(err_nbr);
}
