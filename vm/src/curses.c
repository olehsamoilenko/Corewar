/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curses.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_memory(t_war *war)
{
	int i = -1;
	char s[3];

	while (++i < MEM_SIZE)
	{
		// color on
		if (war->map[i]->color == 0)
		{
			// wattroff(war->win_mem, A_COLOR);
			wattron(war->win_mem, A_BOLD);
			wattron(war->win_mem, COLOR_PAIR(14));
			
		}
		else
			wattron(war->win_mem, COLOR_PAIR(war->map[i]->color));

		// bold
		if (war->map[i]->cycles_bold && war->cycle - war->map[i]->cycles_bold < 50 - 1) 
			wattron(war->win_mem, A_BOLD);

		// live
		if (war->map[i]->cycles_live && war->cycle - war->map[i]->cycles_live < 50 - 1) // 50 ?
		{
			wattron(war->win_mem, A_BOLD);
			wattron(war->win_mem, COLOR_PAIR(8 + war->map[i]->color));
		}


		sprintf(s, "%02x", war->map[i]->value);
		mvwaddstr(war->win_mem, i / 64 + 1, (i % 64) * 3 + 2, s);
		wattroff(war->win_mem, A_BOLD);
	}

	t_carriage *tmp = war->carriages; // need?
	while (tmp)
	{
		if (war->map[tmp->position]->color == 0)
			wattron(war->win_mem, COLOR_PAIR(15));
		else
			wattron(war->win_mem, COLOR_PAIR(4 + war->map[tmp->position]->color)); // number
		sprintf(s, "%02x", war->map[tmp->position]->value);
		mvwaddstr(war->win_mem, tmp->position / 64 + 1, (tmp->position % 64) * 3 + 2, s);
		tmp = tmp->next;
	}
	wrefresh(war->win_mem);

	// info window

	char *itoa;
	
	itoa = ft_itoa(war->cycle);
	mvwaddstr(war->win_info, 2, 10, itoa);
	ft_strdel(&itoa);
	itoa = ft_itoa(war->processes_amount);
	mvwaddstr(war->win_info, 4, 14, itoa);
	ft_strdel(&itoa);
	itoa = ft_itoa(war->champs[0]->last_live);
	mvwaddstr(war->win_info, 7, 37 - ft_strlen(itoa), itoa);
	ft_strdel(&itoa);
	itoa = ft_itoa(war->champs[0]->lives_cur_period);
	mvwaddstr(war->win_info, 8, 37 - ft_strlen(itoa), itoa);
	ft_strdel(&itoa);
	itoa = ft_itoa(war->cycles_to_die);
	mvwaddstr(war->win_info, 10, 17, itoa);
	ft_strdel(&itoa);
	// itoa = ft_itoa(war->cycles_to_die);
	// mvwaddstr(war->win_info, 8, 17, itoa);
	// ft_strdel(&itoa);
	wrefresh(war->win_info);
}

void	define_colors()
{
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

	// live
	init_pair(9, COLOR_WHITE, COLOR_GREEN);
	init_pair(10, COLOR_WHITE, COLOR_BLUE);
	init_pair(11, COLOR_WHITE, COLOR_RED);
	init_pair(12, COLOR_WHITE, COLOR_CYAN);

	// default
	init_pair(13, COLOR_BLACK, COLOR_BLACK);

	// added
	init_pair(14, 8, COLOR_BLACK); // Bright black
	init_pair(15, 8, 8);

}

void	init_curses(t_war *war)
{
	initscr();
	curs_set(0);
	refresh();
	define_colors();
	war->win_mem = newwin(66, 195, 0, 0);
	war->win_info = newwin(20, 60, 0, 197);
	war->win_getch = newwin(10, 60, 21, 197);
	// mvwaddstr(war->win_mem, 5, 0, "hello");
	// wrefresh(war->win_mem);
	box(war->win_mem, 0, 0);
	box(war->win_info, 0, 0);
	box(war->win_getch, 0, 0);
	wattron(war->win_info, A_BOLD);
	mvwaddstr(war->win_info, 2, 2, "Cycle :");
	mvwaddstr(war->win_info, 4, 2, "Processes :");
	mvwaddstr(war->win_info, 6, 2, "Player -1 : ");
	wattron(war->win_info, COLOR_PAIR(1));
	mvwaddstr(war->win_info, 6, 14, war->champs[0]->header->prog_name);
	wattroff(war->win_info, A_COLOR);
	mvwaddstr(war->win_info, 7, 4, "Last live : ");
	mvwaddstr(war->win_info, 8, 4, "Lives in current period : ");
	mvwaddstr(war->win_info, 10, 2, "CYCLE_TO_DIE : ");
	mvwaddstr(war->win_info, 12, 2, "CYCLE_DELTA : ");
	wrefresh(war->win_mem); // need ?
	wrefresh(war->win_info);
	wrefresh(war->win_getch);
	// box(war->win_mem, 0, 0);



	// status("starting", war);
	// print_memory(war);

}

void	status(char *line, t_war *war)
{
	if (war->flag_visual)
	{
		mvwaddstr(war->win_info, 4, 2, "                       ");
		mvwaddstr(war->win_info, 4, 2, line);
		wrefresh(war->win_info);
	}
}

void	over_over(t_war *war)
{
	delwin(war->win_mem);
	delwin(war->win_info);
	endwin();
	system("leaks vm");
	exit(0);
}

void	over_curses(t_war *war)
{
	int key;
	status("overing", war);
	while ((key = wgetch(war->win_getch)) != KEY_ESC)
	{
		// print_memory(war);
		// box(war->win_mem, 0, 0);
		// wrefresh(war->win_mem);
	}
	over_over(war);
}