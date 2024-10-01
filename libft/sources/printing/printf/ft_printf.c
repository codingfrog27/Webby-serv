/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/07 13:31:38 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/09/05 19:20:30 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	flag_checker(char c, va_list *ap)
{
	static int	(*table[128])(va_list *) = \
	{
	['c'] = &print_char, ['s'] = &print_string, ['p'] = &print_p, \
	['d'] = &print_nbr, ['i'] = &print_nbr, ['u'] = &print_uns_nbr, \
	['x'] = &print_x, ['X'] = &print_upper_x
	};

	if (table[(int)c] == NULL)
		return (0);
	return ((*table[(int)c])(ap));
}

int	ft_printf(const char *str, ...)
{
	int		len;
	size_t	i;
	va_list	ap;

	len = 0;
	i = 0;
	va_start(ap, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			if (str[i] == '%')
				len += write(1, "%", 1);
			else
				len += flag_checker(str[i], &ap);
		}
		else
			len += write(1, &str[i], 1);
		i++;
	}
	return (len);
}
