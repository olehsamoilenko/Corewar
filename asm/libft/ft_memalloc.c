/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 15:09:29 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/25 15:09:31 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void *res;

	res = (void *)malloc(sizeof(*res) * size);
	if (res == NULL)
		return (NULL);
	ft_bzero(res, size);
	return (res);
}
