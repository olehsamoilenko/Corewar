/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 14:11:59 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 14:12:01 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static uintmax_t	get_unsigned(va_list arg, t_box *info)
{
	uintmax_t nbr;

	if (ft_strequ(info->length, "l") || info->type == 'U')
		nbr = va_arg(arg, unsigned long);
	else if (ft_strequ(info->length, "hh"))
		nbr = (unsigned char)va_arg(arg, unsigned int);
	else if (ft_strequ(info->length, "h") && info->type == 'u')
		nbr = (unsigned short)va_arg(arg, unsigned int);
	else if (ft_strequ(info->length, "ll"))
		nbr = va_arg(arg, unsigned long long);
	else if (ft_strequ(info->length, "j"))
		nbr = va_arg(arg, uintmax_t);
	else if (ft_strequ(info->length, "z"))
		nbr = (size_t)va_arg(arg, size_t);
	else
		nbr = va_arg(arg, unsigned int);
	free(info->length);
	return (nbr);
}

static void			calc_spaces_uns(t_box *info, char *number)
{
	info->sum_zeroes = info->precision - ft_strlen(number);
	if (info->sum_zeroes < 0)
		info->sum_zeroes = 0;
	info->start = info->width - info->sum_zeroes - ft_strlen(number);
	if (info->start < 0)
		info->start = 0;
	if (info->zero == 1 && info->precision == 0 && info->minus == 0)
	{
		info->sum_zeroes += info->start;
		info->start = 0;
	}
}

int					print_unsigned(va_list arg, t_box *info)
{
	uintmax_t	nbr;
	char		*number;
	char		*temp;
	int			ret;

	nbr = get_unsigned(arg, info);
	number = ft_itoa_base(nbr, 10, 0);
	if (info->precision == -1 && nbr == 0)
	{
		free(number);
		number = ft_strdup("");
		info->precision = 0;
	}
	calc_spaces_uns(info, number);
	while (info->sum_zeroes-- > 0)
	{
		temp = number;
		number = ft_strjoin("0", number);
		free(temp);
	}
	number = output(info, number);
	ret = ft_putstr_fd(number, g_fd);
	free(number);
	return (ret);
}
