/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curses_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:36:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/11 19:36:53 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_name_champs(t_war *war)
{
	int		player;
	char	*sub_name;

	player = -1;
	while (++player < 4)
	{
		if (war->champs[player] != NULL)
		{
			wattroff(war->win_info, A_COLOR);
			mvwprintw(war->win_info, 7 + 4 * war->champs[player]->number, 3,
								"Player -%d : ", war->champs[player]->number);
			wattron(war->win_info, COLOR_PAIR(war->champs[player]->number));
			sub_name = ft_strsub(war->champs[player]->header->prog_name,
																	0, 41);
			mvwaddstr(war->win_info, 7 + 4 * war->champs[player]->number,
																15, sub_name);
			ft_strdel(&sub_name);
			wattroff(war->win_info, A_COLOR);
		}
	}
}

void	print_info_players(t_war *war)
{
	int player;

	print_name_champs(war);
	player = -1;
	while (++player < 4)
	{
		if (war->champs[player] != NULL)
		{
			mvwprintw(war->win_info, 7 + 4 * war->champs[player]->number + 1, 5,
							"Last live : %21d", war->champs[player]->last_live);
			mvwprintw(war->win_info, 7 + 4 * war->champs[player]->number + 2, 5,
		"Lives in current period : %7d", war->champs[player]->lives_cur_period);
			if (7 + 4 * war->champs[player]->number + 2 > war->last_line)
				war->last_line = 7 + 4 * war->champs[player]->number + 2;
		}
	}
}

void	print_info(t_war *war)
{
	wresize(war->win_mem, 68, 197);
	wresize(war->win_info, 68, 60);
	wattron(war->win_mem, COLOR_PAIR(15));
	wattron(war->win_info, COLOR_PAIR(15));
	box(war->win_mem, '*', '*');
	box(war->win_info, '*', '*');
	wattroff(war->win_mem, A_COLOR);
	wattroff(war->win_info, A_COLOR);
	if (war->flag_run == false)
		mvwaddstr(war->win_info, 2, 3, "** STOP **");
	else
		mvwaddstr(war->win_info, 2, 3, " ** RUN **");
	mvwprintw(war->win_info, 4, 3, "Cycles per second : %d  ",
													war->cycles_in_second);
	mvwprintw(war->win_info, 7, 3, "Cycle : %d", war->cycle);
	mvwprintw(war->win_info, 9, 3, "Processes : %d             ",
													war->processes_amount);
	wattron(war->win_info, A_BOLD);
	print_info_players(war);
	mvwprintw(war->win_info, war->last_line + 2, 3, "CYCLE_TO_DIE : %4d/%d   ",
	war->cycles_after_check, war->cycles_to_die);
	mvwprintw(war->win_info, war->last_line + 4, 3, "MAX_CHECKS : %6d/%d   ",
	war->checks, MAX_CHECKS);
	wrefresh(war->win_info);
}

void	print_bytes(t_war *war)
{
	int i;

	i = -1;
	while (++i < MEM_SIZE)
	{
		if (war->map[i]->color == 0)
		{
			wattron(war->win_mem, A_BOLD);
			wattron(war->win_mem, COLOR_PAIR(14));
		}
		else
			wattron(war->win_mem, COLOR_PAIR(war->map[i]->color));
		if (war->map[i]->cycles_bold &&
								war->cycle - war->map[i]->cycles_bold < 50 - 1)
			wattron(war->win_mem, A_BOLD);
		if (war->map[i]->cycles_live &&
								war->cycle - war->map[i]->cycles_live < 50 - 1)
		{
			wattron(war->win_mem, A_BOLD);
			wattron(war->win_mem, COLOR_PAIR(8 + war->map[i]->color));
		}
		mvwprintw(war->win_mem, i / 64 + 2, (i % 64) * 3 + 3,
													"%02x", war->map[i]->value);
		wattroff(war->win_mem, A_BOLD);
	}
}

void	print_memory(t_war *war)
{
	t_carriage *tmp;

	print_bytes(war);
	tmp = war->carriages;
	while (tmp)
	{
		if (war->map[tmp->position]->color == 0)
			wattron(war->win_mem, COLOR_PAIR(15));
		else
			wattron(war->win_mem,
								COLOR_PAIR(4 + war->map[tmp->position]->color));
		mvwprintw(war->win_mem, tmp->position / 64 + 2,
		(tmp->position % 64) * 3 + 3, "%02x", war->map[tmp->position]->value);
		tmp = tmp->next;
	}
	wrefresh(war->win_mem);
}
