/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 10:51:40 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/26 10:51:44 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(const char *str)
{
	int i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!(str = ft_strnew(ft_len(s1) + ft_len(s2))))
		return (NULL);
	if (s1)
		while (s1[i])
		{
			str[j] = s1[i];
			i++;
			j++;
		}
	i = 0;
	if (s2)
		while (s2[i])
		{
			str[j] = s2[i];
			i++;
			j++;
		}
	str[j] = '\0';
	return (str);
}
