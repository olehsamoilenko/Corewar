/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 15:29:24 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/04/30 15:29:26 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_box	fill_flags(const char *format, int *i, t_box *info)
{
	int		start;
	char	*temp;

	while (is_flag(format[*i]))
	{
		put_flag(info, format[*i]);
		(*i)++;
	}
	while (ft_isdigit(format[*i]))
	{
		start = *i;
		while (ft_isdigit(format[*i]))
			(*i)++;
		temp = ft_strsub(format, start, *i - start);
		info->width = ft_atoi(temp);
		free(temp);
	}
	while (is_flag(format[*i]))
	{
		put_flag(info, format[*i]);
		(*i)++;
	}
	return (*info);
}

static t_box	fill_precision(const char *format, int *i, t_box *info)
{
	int		start;
	char	*temp;

	if (format[*i] == '.')
	{
		(*i)++;
		start = *i;
		while (ft_isdigit(format[*i]))
			(*i)++;
		temp = ft_strsub(format, start, *i - start);
		info->precision = ft_atoi(temp);
		free(temp);
		if (info->precision == 0)
			info->precision = -1;
	}
	if (is_length(format[*i]))
	{
		start = *i;
		while (is_length(format[*i]))
			(*i)++;
		info->length = ft_strsub(format, start, *i - start);
	}
	return (*info);
}

static int		print_spaces(t_box *info)
{
	int ret;

	ret = 0;
	if (info->zero == 1 && info->minus == 0)
	{
		while (--info->width > 0)
			ret += write(g_fd, "0", 1);
	}
	if (info->minus == 1)
	{
		ret += write(g_fd, &info->type, 1);
		while (--info->width > 0)
			ret += write(g_fd, " ", 1);
	}
	else
	{
		while (--info->width > 0)
			ret += write(g_fd, " ", 1);
		ret += write(g_fd, &info->type, 1);
	}
	return (ret);
}

static int		printf_help(const char *format, int i, int *ret, va_list arg)
{
	t_box *info;

	info = (t_box *)malloc(sizeof(t_box));
	reset_box(info);
	*info = fill_flags(format, &i, info);
	*info = fill_precision(format, &i, info);
	if (format[i] == '%')
		*ret += print_percent(info);
	if (is_correct_type(format[i]))
	{
		info->type = format[i];
		*ret += print(arg, info);
	}
	else if (format[i] != '%')
	{
		info->type = format[i];
		*ret += print_spaces(info);
	}
	free(info);
	return (i);
}

int				ft_printf(const char *format, ...)
{
	va_list		arg;
	int			i;
	int			ret;

	va_start(arg, format);
	ret = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == '\0')
				break ;
			i = printf_help(format, i, &ret, arg);
		}
		else
		{
			ret++;
			write(g_fd, &format[i], 1);
		}
		i++;
	}
	return (ret);
}
