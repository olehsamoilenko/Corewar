/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:49:07 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/04/30 18:49:09 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(uintmax_t n, int base)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		i++;
		n /= base;
	}
	return (i);
}

char		*ft_itoa_base(uintmax_t n, int base, int flag)
{
	char	*str;
	int		len;
	char	*digits;
	int		i;

	if (n <= 0)
		return (ft_strdup("0"));
	len = ft_count(n, base);
	str = (char *)malloc(sizeof(char) * (len + 1));
	digits = "0123456789abcdef";
	str[len] = '\0';
	while (n != 0)
	{
		str[--len] = digits[n % base];
		n /= base;
	}
	if (flag == 1)
	{
		i = -1;
		while (str[++i])
			str[i] = ft_toupper(str[i]);
	}
	return (str);
}
