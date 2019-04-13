/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_key(int key, t_war *war)
{
	if (key == KEY_ESC)
		over_over(war);
	if (key == KEY_W)
	{
		if (war->cycles_in_second > 1)
			war->cycles_in_second -= 1;
	}
	if (key == KEY_E)
		war->cycles_in_second += 1;
	if (key == KEY_SPACE)
		war->flag_run = (war->flag_run == true) ? false : true;
}

t_bool	cyc_inc_alowed(int key, t_war *war)
{
	t_bool success;

	success = false;
	if (!war->flag_visual || key == KEY_S ||
	war->cycle < war->flag_dump || (war->flag_run &&
	((float)clock() / CLOCKS_PER_SEC - war->time >=
	1.0 / war->cycles_in_second)))
	{
		war->cycle += 1;
		success = true;
		if (war->flag_visual &&
		(!war->flag_run ||
		war->cycle - war->last_print >= war->cycles_in_second))
		{
			war->time = (float)clock() / CLOCKS_PER_SEC;
			war->last_print = war->cycle;
		}
		war->time += 1.0 / war->cycles_in_second;
	}
	return (success);
}

t_bool	next_cycle(t_war *war)
{
	int		key;
	t_bool	success;

	key = 0;
	if (war->flag_visual && war->cycle >= war->flag_dump)
	{
		handle_key(key = getch(), war);
		print_info(war);
	}
	success = cyc_inc_alowed(key, war);
	if (war->flag_visual)
	{
		if (war->cycle >= war->flag_dump)
			print_memory(war);
		print_info(war);
	}
	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("It is now cycle %d\n", war->cycle);
	return (success);
}
