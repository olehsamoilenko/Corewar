/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_war	*init()
{
	t_war *war = ft_memalloc(sizeof(t_war));
	int i = -1;

	
	while (++i < MEM_SIZE)
	{
		war->map[i] = ft_memalloc(sizeof(t_mem_cell));
		war->map[i]->value = 0x00; // fake 
	}
	war->carriages = NULL;
	war->flag_dump = -1;
	war->cycles_to_die = CYCLE_TO_DIE;
	// war->cycles_after_check = 0;
	war->flag_run = false;
	war->cycles_in_second = 50;
	war->last_line = 0;
	return (war);
}