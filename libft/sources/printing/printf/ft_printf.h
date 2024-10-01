/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/07 13:32:46 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/06/22 14:52:23 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "libft.h"
# include <stdbool.h>

int	ft_printf(const char *str, ...);
int	print_char(va_list *ap);
int	print_string(va_list *ap);
int	print_nbr(va_list *ap);
int	print_uns_nbr(va_list *ap);
int	put_unbr(unsigned int nbr);
int	hex_convert(unsigned long n, bool is_upper);
int	print_p(va_list *ap);
int	print_x(va_list *ap);
int	print_upper_x(va_list *ap);

#endif