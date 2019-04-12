/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_uni_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 16:35:17 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 16:35:19 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int		uni_putstr(int *string, t_box *info)
{
	int temp;
	int i;

	i = 0;
	temp = 0;
	while (string[i] != '\0')
	{
		if (info->precision != 0)
		{
			if (temp + sizeof_sym(string[i]) <= info->precision)
			{
				ft_putchar(string[i]);
				temp += sizeof_sym(string[i]);
			}
		}
		else
		{
			ft_putchar(string[i]);
			temp += sizeof_sym(string[i]);
		}
		i++;
	}
	return (temp);
}

static int		null_str(t_box *info)
{
	int		ret;
	char	*string;
	char	*temp;

	string = ft_strdup("(null)");
	if (info->precision == -1)
	{
		free(string);
		string = ft_strdup("");
	}
	if (info->precision > 0)
	{
		temp = ft_strsub(string, 0, info->precision);
		free(string);
		string = temp;
	}
	ret = output_str(info, string);
	return (ret);
}

static t_box	*calc_size(int *string, t_box *info)
{
	int size;
	int i;

	i = 0;
	size = 0;
	while (string[i] != '\0')
	{
		if (info->precision != 0)
		{
			if (size + sizeof_sym(string[i]) <= info->precision)
			{
				size += sizeof_sym(string[i]);
			}
		}
		i++;
	}
	if (info->precision > size)
		info->start += info->precision - size;
	return (info);
}

static int		out_uni_str(t_box *info, int *string, int length)
{
	int temp;
	int ret;

	ret = 0;
	temp = 0;
	if (info->precision > 0)
		info->start = info->width - info->precision;
	else if (info->precision == -1)
		info->start = info->width;
	else
		info->start = info->width - length;
	info = calc_size(string, info);
	if (info->minus == 1)
	{
		temp += uni_putstr(string, info);
		while (info->start-- > 0)
			ret += write(g_fd, " ", 1);
	}
	else
	{
		while (info->start-- > 0)
			info->zero ? ret += write(1, "0", 1) : (ret += write(1, " ", 1));
		temp += uni_putstr(string, info);
	}
	return (ret + temp);
}

int				print_uni_string(va_list arg, t_box *info)
{
	int		*string;
	int		length;
	int		i;
	int		ret;

	ret = 0;
	string = va_arg(arg, int*);
	length = 0;
	i = 0;
	if (string == NULL)
		return (null_str(info));
	else
	{
		while (string[i] != '\0')
		{
			length += sizeof_sym(string[i]);
			i++;
		}
		ret = out_uni_str(info, string, length);
	}
	return (ret);
}
