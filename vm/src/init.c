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

void	init_curses(t_war *war)
{
	initscr();
	curs_set(0);
	refresh();
	war->win_mem = newwin(66, 195, 0, 0);
	war->win_info = newwin(10, 30, 0, 197);
	war->win_getch = newwin(10, 30, 11, 197);
	// mvwaddstr(war->win_mem, 5, 0, "hello");
	// wrefresh(war->win_mem);
	box(war->win_mem, 0, 0);
	box(war->win_info, 0, 0);
	box(war->win_getch, 0, 0);
	mvwaddstr(war->win_info, 2, 2, "Cycle :");
	wrefresh(war->win_mem); // need ?
	wrefresh(war->win_info);
	wrefresh(war->win_getch);
	// box(war->win_mem, 0, 0);

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	// carriage
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_RED);
	init_pair(8, COLOR_BLACK, COLOR_CYAN);

	init_pair(9, COLOR_BLACK, COLOR_BLACK);

	// status("starting", war);
	// print_memory(war);

}

t_war	*init()
{
	t_war *war = ft_memalloc(sizeof(t_war));
	int i = -1;

	
	while (++i < MEM_SIZE)
	{
		war->map[i] = ft_memalloc(sizeof(t_mem_cell));
		war->map[i]->value = 0xFF; // fake 
	}
	war->carriages = NULL;
	war->flag_dump = -1;
	return (war);
}