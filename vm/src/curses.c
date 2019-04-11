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

void	define_colors(void)
{
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_RED);
	init_pair(8, COLOR_BLACK, COLOR_CYAN);
	init_pair(9, COLOR_WHITE, COLOR_GREEN);
	init_pair(10, COLOR_WHITE, COLOR_BLUE);
	init_pair(11, COLOR_WHITE, COLOR_RED);
	init_pair(12, COLOR_WHITE, COLOR_CYAN);
	init_pair(13, COLOR_BLACK, COLOR_BLACK);
	init_pair(14, 8, COLOR_BLACK);
	init_pair(15, 8, 8);
}

void	init_curses(t_war *war)
{
	initscr();
	curs_set(false);
	noecho();
	nodelay(stdscr, true);
	refresh();
	define_colors();
	war->win_mem = newwin(68, 197, 0, 0);
	war->win_info = newwin(68, 60, 0, 196);
	wrefresh(war->win_mem);
	wrefresh(war->win_info);
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

	print_memory(war);
	print_info(war);
	mvwaddstr(war->win_info, war->last_line + 4, 3, "* WINNER * - ");
	wattron(war->win_info, COLOR_PAIR(war->last_live->number));
	mvwaddstr(war->win_info, war->last_line + 4, 16,
								war->last_live->header->prog_name);
	wattroff(war->win_info, A_COLOR);
	wrefresh(war->win_info);
	while ((key = getch()) != KEY_ESC)
		;
	over_over(war);
}
