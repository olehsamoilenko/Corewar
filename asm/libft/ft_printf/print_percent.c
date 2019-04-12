/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_percent.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 15:06:14 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 15:06:17 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	print_percent(t_box *info)
{
	char	*string;
	int		ret;
	char	*temp;

	string = ft_strnew(2);
	string[0] = '%';
	if ((info->start = info->width - 1) < 0)
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
	ret = ft_putstr_fd(string, g_fd);
	free(string);
	return (ret);
}
