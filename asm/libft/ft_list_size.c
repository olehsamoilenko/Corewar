/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 13:37:48 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/31 13:37:50 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_list_size(t_list *begin_list)
{
	int size;

	size = 0;
	if (!begin_list)
		return (0);
	while (begin_list)
	{
		size++;
		begin_list = begin_list->next;
	}
	return (size);
}
