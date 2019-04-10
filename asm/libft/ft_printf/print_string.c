/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 15:09:24 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 15:09:26 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int		output_str(t_box *info, char *string)
{
	char	*temp;
	int		ret;

	info->start = info->width - ft_strlen(string);
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
	ret = ft_putstr_fd(string, g_fd);
	free(string);
	return (ret);
}

int		print_string(va_list arg, t_box *info)
{
	char	*temp;
	char	*string;

	if (ft_strequ(info->length, "l"))
	{
		free(info->length);
		return (print_uni_string(arg, info));
	}
	temp = va_arg(arg, char*);
	string = ft_strdup(temp);
	if (temp == 0)
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
	return (output_str(info, string));
}
