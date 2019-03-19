/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 10:25:55 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/22 10:25:58 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int		pos;
	char	*dst;
	int		i;
	int		j;

	pos = 0;
	dst = (char *)haystack;
	while (dst[pos])
	{
		j = 0;
		i = pos;
		while (needle[j] == dst[i] && needle[j])
		{
			j++;
			i++;
		}
		if (!needle[j])
			return (&dst[pos]);
		pos++;
	}
	if (*dst == '\0' && *needle == '\0')
		return (&dst[pos]);
	return (0);
}
