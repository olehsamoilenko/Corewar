/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 08:53:13 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/03/28 08:53:14 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	if (!alst || !del || !(*alst))
		return ;
	if ((*alst)->next)
	{
		ft_lstdel(&(*alst)->next, del);
	}
	ft_lstdelone(alst, del);
}
