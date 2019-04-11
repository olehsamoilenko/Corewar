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

void	print_info(t_war *war)
{
	char *itoa;

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

	mvwprintw(war->win_info, 4, 3, "Cycles in second : %d  ", war->cycles_in_second);

	mvwprintw(war->win_info, 7, 3, "Cycle : %d", war->cycle);
	mvwprintw(war->win_info, 9, 3, "Processes : %d             ", war->processes_amount);

	wattron(war->win_info, A_BOLD);
	int player = 0;
	int line = 7;
	while (player < 4)
	{
		if (war->champs[player] != NULL)
		{
			wattroff(war->win_info, A_COLOR);
			mvwprintw(war->win_info, line + 4 * war->champs[player]->number, 3, "Player -%d : ", war->champs[player]->number);
			wattron(war->win_info, COLOR_PAIR(war->champs[player]->number));
			char *sub_name = ft_strsub(war->champs[player]->header->prog_name, 0, 41);
			mvwaddstr(war->win_info, line + 4 * war->champs[player]->number, 15, sub_name);
			ft_strdel(&sub_name);
			// line += 4;
			wattroff(war->win_info, A_COLOR);		
		}
		player++;
	}

	 player = 0;
	 line = 7;
	while (player < 4)
	{
		if (war->champs[player] != NULL)
		{
			mvwprintw(war->win_info, line + 4 * war->champs[player]->number + 1, 5, "Last live : %21d", war->champs[player]->last_live);
			mvwprintw(war->win_info, line + 4 * war->champs[player]->number + 2, 5, "Lives in current period : %7d", war->champs[player]->lives_cur_period);
			if (line + 4 * war->champs[player]->number + 2 > war->last_line)
				war->last_line = line + 4 * war->champs[player]->number + 2;
		}
		player++;
	}
	mvwprintw(war->win_info, war->last_line + 2, 3, "CYCLE_TO_DIE : %d              ", war->cycles_to_die);
	wrefresh(war->win_info);
}

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
		mvwaddstr(war->win_mem, i / 64 + 2, (i % 64) * 3 + 3, s);
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
		mvwaddstr(war->win_mem, tmp->position / 64 + 2, (tmp->position % 64) * 3 + 3, s);
		tmp = tmp->next;
	}
	wrefresh(war->win_mem);
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
	curs_set(false);
	noecho();
	nodelay(stdscr, true);
	refresh();
	// cbreak();
	define_colors();
	war->win_mem = newwin(68, 197, 0, 0);
	war->win_info = newwin(68, 60, 0, 196);

	wrefresh(war->win_mem); // need ?
	wrefresh(war->win_info);
	// wrefresh(war->win_getch);
}

void	over_over(t_war *war)
{
	delwin(war->win_mem);
	delwin(war->win_info);
	endwin();
	system("leaks corewar");
	exit(0);
}

void	over_curses(t_war *war)
{
	int key;
	// status("overing", war);
	mvwaddstr(war->win_info, war->last_line + 4, 3, "* WINNER * - ");
	wattron(war->win_info, COLOR_PAIR(war->last_live->number));
	mvwaddstr(war->win_info, war->last_line + 4, 16, war->last_live->header->prog_name);
	wattroff(war->win_info, A_COLOR);
	wrefresh(war->win_info);	
	
	while ((key = getch()) != KEY_ESC)
	{

	}
	over_over(war);
}
