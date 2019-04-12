/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 08:16:11 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/23 08:16:13 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t	i;
	char	*s_dst;
	char	*s_src;

	s_dst = (char *)dst;
	s_src = (char *)src;
	i = 0;
	while (i < n)
	{
		s_dst[i] = s_src[i];
		if (s_src[i] == (char)c)
			return (&(s_dst[i + 1]));
		i++;
	}
	if (i == n)
		return (NULL);
	return ((void *)s_dst);
}
