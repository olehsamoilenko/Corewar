/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinteger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 18:00:27 by osamoile          #+#    #+#             */
/*   Updated: 2018/03/21 18:00:28 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isinteger(char *line)
{
	int num = ft_atoi(line);
	char *itoa = ft_itoa(num);
	int res;
	if (ft_strequ(itoa, line))
		res = 1;
	else
		res = 0;
	ft_strdel(&itoa);
	return (res);
}
