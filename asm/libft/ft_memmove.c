/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 08:42:27 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/23 08:42:29 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*s_dst;
	unsigned char	*s_src;

	i = 0;
	s_dst = (unsigned char *)dst;
	s_src = (unsigned char *)src;
	if (s_src < s_dst)
	{
		while (len-- > 0)
			s_dst[len] = s_src[len];
	}
	else
	{
		while (i < len)
		{
			s_dst[i] = s_src[i];
			i++;
		}
	}
	return ((void *)s_dst);
}
