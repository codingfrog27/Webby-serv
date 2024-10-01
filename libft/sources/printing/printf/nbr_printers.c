/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nbr_printers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/09 13:11:38 by mde-cloe      #+#    #+#                 */
/*   Updated: 2022/06/22 14:48:38 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_nbr(va_list *ap)
{
	char	*nstr;
	size_t	len;

	nstr = ft_itoa(va_arg(*ap, int));
	len = ft_strlen(nstr);
	write(1, nstr, len);
	free(nstr);
	return (len);
}

int	put_unbr(unsigned int nbr)
{
	if (nbr > 9)
		return (put_unbr(nbr / 10) + put_unbr(nbr % 10));
	nbr += '0';
	return (write(1, &nbr, 1));
}

int	print_uns_nbr(va_list *ap)
{
	unsigned int	nbr;

	nbr = va_arg(*ap, unsigned int);
	return (put_unbr(nbr));
}
