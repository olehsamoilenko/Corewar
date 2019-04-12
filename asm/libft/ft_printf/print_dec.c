/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 13:09:15 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 13:09:17 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static intmax_t	get_nbr(va_list arg, t_box *info)
{
	intmax_t nbr;

	if (ft_strequ(info->length, "l") || info->type == 'D')
		nbr = va_arg(arg, long);
	else if (ft_strequ(info->length, "hh"))
		nbr = (char)va_arg(arg, int);
	else if (ft_strequ(info->length, "h"))
		nbr = (short)va_arg(arg, int);
	else if (ft_strequ(info->length, "ll"))
		nbr = va_arg(arg, long long);
	else if (ft_strequ(info->length, "j"))
		nbr = va_arg(arg, intmax_t);
	else if (ft_strequ(info->length, "z"))
		nbr = (size_t)va_arg(arg, size_t);
	else
		nbr = va_arg(arg, int);
	free(info->length);
	return (nbr);
}

static t_box	*calc_spaces(t_box *info, char *number, intmax_t nbr)
{
	if ((info->sum_zeroes = info->precision - ft_strlen(number)) < 0)
		info->sum_zeroes = 0;
	info->start = info->width - info->sum_zeroes - ft_strlen(number) - \
				ft_strlen(info->prefix);
	if (info->start < 0)
		info->start = 0;
	if (info->space == 1 && nbr >= 0)
	{
		info->prefix = " ";
		info->start -= 1;
	}
	if (info->plus == 1 && nbr >= 0)
	{
		info->prefix = "+";
		info->start -= 1;
	}
	if (info->zero == 1 && info->precision == 0 && info->minus == 0)
	{
		info->sum_zeroes += info->start;
		info->start = 0;
	}
	return (info);
}

static char		*change_num(char *number, t_box *info, intmax_t nbr)
{
	char *temp;

	temp = number;
	if (info->precision == -1 && nbr == 0)
	{
		temp = ft_strdup("");
		info->precision = 0;
		free(number);
	}
	if (nbr < 0)
	{
		info->prefix = "-";
		temp = ft_strsub(number, 1, ft_strlen(number) - 1);
		free(number);
	}
	return (temp);
}

char			*output(t_box *info, char *number)
{
	char	*temp;

	if (info->minus)
	{
		while (info->start-- > 0)
		{
			temp = number;
			number = ft_strjoin(number, " ");
			free(temp);
		}
	}
	else
	{
		while (info->start-- > 0)
		{
			temp = number;
			number = ft_strjoin(" ", number);
			free(temp);
		}
	}
	return (number);
}

int				print_dec(va_list arg, t_box *info)
{
	intmax_t	nbr;
	char		*number;
	char		*temp;
	int			ret;

	nbr = get_nbr(arg, info);
	number = ft_itoa(nbr);
	number = change_num(number, info, nbr);
	info = calc_spaces(info, number, nbr);
	while (info->sum_zeroes-- > 0)
	{
		temp = number;
		number = ft_strjoin("0", number);
		free(temp);
	}
	if (info->prefix != 0)
	{
		temp = number;
		number = ft_strjoin(info->prefix, number);
		free(temp);
	}
	number = output(info, number);
	ret = ft_putstr_fd(number, g_fd);
	free(number);
	return (ret);
}
