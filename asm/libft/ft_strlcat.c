/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 08:40:27 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/22 08:40:29 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int i;
	unsigned int len;
	unsigned int src_len;

	i = 0;
	len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dstsize > 0)
	{
		while ((i + len < (dstsize - 1)) && src[i])
		{
			dst[i + len] = src[i];
			i++;
		}
	}
	dst[i + len] = '\0';
	if (len < dstsize)
		return (len + src_len);
	else
		return (src_len + dstsize);
}
