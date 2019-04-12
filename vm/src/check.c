/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	remove_start(t_carriage **list, t_war *war)
{
	t_carriage *del;

	while (*list && (war->cycle - (*list)->last_live >= war->cycles_to_die ||
	war->cycles_to_die <= 0))
	{
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
				(*list)->number, war->cycle - (*list)->last_live,
				war->cycles_to_die);
		del = *list;
		*list = (*list)->next;
		free(del);
	}
}

void	remove_carriages(t_carriage **list, t_war *war)
{
	t_carriage *del;
	t_carriage *tmp;

	remove_start(list, war);
	tmp = *list;
	while (tmp && tmp->next)
	{
		if (war->cycle - tmp->next->last_live >= war->cycles_to_die ||
													war->cycles_to_die <= 0)
		{
			if (war->flag_verbose && war->cycle >= war->flag_dump)
				ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
					tmp->next->number, war->cycle - tmp->next->last_live,
														war->cycles_to_die);
			del = tmp->next;
			tmp->next = tmp->next->next;
			free(del);
		}
		else
			tmp = tmp->next;
	}
}

void	check(t_war *war)
{
	war->cycles_after_check += 1;
	if (war->cycles_after_check >= war->cycles_to_die)
	{
		war->checks += 1;
		remove_carriages(&war->carriages, war);
		if (war->all_lives >= NBR_LIVE || war->checks == MAX_CHECKS)
		{
			war->cycles_to_die -= CYCLE_DELTA;
			war->checks = 0;
			if (war->flag_verbose && war->cycle >= war->flag_dump)
				ft_printf("Cycle to die is now %d\n", war->cycles_to_die);
		}
		war->cycles_after_check = 0;
		war->champs[0]->lives_cur_period = 0;
		war->all_lives = 0;
	}
}
