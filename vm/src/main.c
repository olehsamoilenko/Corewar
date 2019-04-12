/*                                                                            */
/* ************************************************************************** */
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

void	error(char *message) // .h
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(message, 2);
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

int		chmps_count(t_champion **champs)
{
	int count = 0;
	int i = -1;
	while (champs[++i] != NULL)
		count++;
	return (count);
}

void	introduce(t_champion **champs, t_bool flag_visual) // fix
{
	int i;

	if (!flag_visual)
	{
		i = -1;
		ft_printf("Introducing contestants...\n");
		while (champs[++i])
		{
			ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
				champs[i]->number, champs[i]->header->prog_size,
				champs[i]->header->prog_name, champs[i]->header->comment);
		}
	}
}

int		main(int argc, char **argv) 
{
	t_war *war;

	parse_params(argc, argv, war = init());
	parse_champions(war);
	introduce(war->champs, war->flag_visual);
	init_curses(war);
	if (war->flag_dump == 0)
		dump(war);
	while (true)
	{
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
	}
	if (war->flag_visual)
		over_curses(war);
	return (0);
}
