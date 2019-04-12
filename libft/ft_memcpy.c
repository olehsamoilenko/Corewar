/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 12:06:14 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/21 12:06:20 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*s_dst;
	char	*s_src;

	i = 0;
	s_dst = (char *)dst;
	s_src = (char *)src;
	while (i < n)
	{
		s_dst[i] = s_src[i];
		i++;
	}
	return ((void *)dst);
}
