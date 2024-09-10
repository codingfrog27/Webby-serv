/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   char_printers.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/07 17:00:46 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/06/09 18:54:25 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(va_list *ap)
{
	ft_putchar_fd(va_arg(*ap, int), 1);
	return (1);
}

int	print_string(va_list *ap)
{
	char	*str;

	str = va_arg(*ap, char *);
	if (!str)
		return (write(1, "(null)", 6));
	return (write(1, str, ft_strlen(str)));
}
