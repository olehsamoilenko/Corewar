/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 16:16:00 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 16:16:02 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static void	output_char(int c, t_box *info, char *string)
{
	if (c == 0)
	{
		if (info->minus)
		{
			write(g_fd, "", 1);
			ft_putstr(string);
		}
		else
		{
			ft_putstr(string);
			write(g_fd, "", 1);
		}
	}
	else
		ft_putstr(string);
}

static char	*help_print(t_box *info, int c, char *string)
{
	char *temp;

	if ((info->start = info->width - sizeof_sym(c)) < 0)
		info->start = 0;
	if (info->zero == 1 && info->minus == 0)
	{
		info->sum_zeroes += info->start;
		info->start = 0;
	}
	while (info->sum_zeroes-- > 0)
	{
		temp = string;
		string = ft_strjoin("0", string);
		free(temp);
	}
	string = output(info, string);
	output_char(c, info, string);
	return (string);
}

static int	output_uni(t_box *info, unsigned int c, int ret)
{
	if (info->zero == 1)
		while (info->sum_zeroes-- > 0)
			ret += write(g_fd, "0", 1);
	if (info->minus == 1)
	{
		ft_putchar(c);
		while (info->start-- > 0)
			ret += write(g_fd, " ", 1);
	}
	else
	{
		while (info->start-- > 0)
			ret += write(g_fd, " ", 1);
		ft_putchar_fd(c, g_fd);
	}
	return (ret);
}

int			print_uni_char(va_list arg, t_box *info)
{
	unsigned int	c;
	int				ret;

	ret = 0;
	c = va_arg(arg, unsigned int);
	if (c != 0)
		ret += sizeof_sym(c);
	if ((info->start = info->width - sizeof_sym(c)) < 0)
		info->start = 0;
	if (info->zero == 1 && info->minus == 0)
	{
		info->sum_zeroes += info->start;
		info->start = 0;
	}
	ret = output_uni(info, c, ret);
	return ((c == 0) ? (ret + 1) : ret);
}

int			print_char(va_list arg, t_box *info)
{
	char	c;
	char	*string;
	int		ret;

	if (ft_strequ(info->length, "l") || ft_strequ(info->length, "ll"))
	{
		free(info->length);
		return (print_uni_char(arg, info));
	}
	c = (char)va_arg(arg, int);
	string = ft_strnew(2);
	string[0] = c;
	string = help_print(info, c, string);
	if (c == 0)
		ret = ft_strlen(string) + 1;
	else
		ret = ft_strlen(string);
	free(string);
	return (ret);
}
