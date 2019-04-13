/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_war	*init(void)
{
	t_war	*war;
	int		i;

	i = -1;
	war = ft_memalloc(sizeof(t_war));
	while (++i < MEM_SIZE)
	{
		war->map[i] = ft_memalloc(sizeof(t_map_cell));
		war->map[i]->value = 0x00;
	}
	war->carriages = NULL;
	war->flag_dump = -1;
	war->cycles_to_die = CYCLE_TO_DIE;
	g_fd = 1;
	war->flag_run = false;
	war->cycles_in_second = 50;
	return (war);
}

void	error(char *message)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(message, 2);
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

void	usage(void)
{
	ft_printf("Usage: ./corewar <[-n number] champion1.cor> <...> [-verbose] \
									[-visual] [-dev] [-dump cycle_number]\n");
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

int		main(int argc, char **argv)
{
	t_war *war;

	parse_params(argc, argv, war = init());
	parse_champions(war);
	throw_basic_carriages(war);
	introduce(war, war->champs, war->flag_visual);
	init_curses(war);
	if (war->flag_dump == 0)
		dump(war);
	while (true)
		if (next_cycle(war))
		{
			run_carriages(war);
			check(war);
			if (!war->carriages)
			{
				verbose_won(war);
				break ;
			}
			if (war->cycle == war->flag_dump)
				dump(war);
		}
	over_curses(war);
	return (0);
}
