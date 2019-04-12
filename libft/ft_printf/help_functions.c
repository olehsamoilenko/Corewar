/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 17:52:01 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/29 17:52:03 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

void	reset_box(t_box *info)
{
	info->minus = 0;
	info->plus = 0;
	info->space = 0;
	info->zero = 0;
	info->hash = 0;
	info->width = 0;
	info->precision = 0;
	info->length = 0;
	info->type = 0;
	info->start = 0;
	info->sum_zeroes = 0;
	info->prefix = 0;
}

void	put_flag(t_box *info, char c)
{
	if (c == '-')
		info->minus = 1;
	else if (c == '+')
		info->plus = 1;
	else if (c == ' ')
		info->space = 1;
	else if (c == '0')
		info->zero = 1;
	else if (c == '#')
		info->hash = 1;
}

int		sizeof_sym(int c)
{
	if (c < 128)
		return (1);
	else if (c < 2048)
		return (2);
	else if (c < 65536)
		return (3);
	else
		return (4);
}

int		print(va_list arg, t_box *info)
{
	int ret;

	ret = 0;
	if (info->type == 'd' || info->type == 'i' || info->type == 'D')
		ret = print_dec(arg, info);
	else if (info->type == 'u' || info->type == 'U')
		ret = print_unsigned(arg, info);
	else if (info->type == 'x' || info->type == 'X' || info->type == 'p')
		ret = print_hex(arg, info);
	else if (info->type == 'o' || info->type == 'O')
		ret = print_octal(arg, info);
	else if (info->type == 'c')
		ret = print_char(arg, info);
	else if (info->type == 'C')
		ret = print_uni_char(arg, info);
	else if (info->type == 's')
		ret = print_string(arg, info);
	else if (info->type == 'S')
		ret = print_uni_string(arg, info);
	return (ret);
}
