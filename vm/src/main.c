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

#include "lemin.h"

static void		flags_handle(int argc, char **argv, t_lem *lem)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-c"))
			lem->flag_color = 1;
		else if (ft_strequ(argv[i], "-p"))
			lem->flag_pathes = 1;
		else if (ft_strequ(argv[i], "-s"))
			lem->flag_steps = 1;
		else
		{
			ft_putendl("Usage: ./lem-in [-c][-p][-s] < map");
			ft_putendl("\t-c\tcolors");
			ft_putendl("\t-p\tlist of unique pathes");
			ft_putendl("\t-s\ttotal number of steps");
			exit(0);
		}
	}
}

static t_lem	*init(void)
{
	t_lem *lem;

	lem = ft_memalloc(sizeof(t_lem));
	lem->nodes = NULL;
	lem->start = NULL;
	lem->end = NULL;
	lem->flag_color = 0;
	return (lem);
}

static int		total_steps(t_list_of_pathes *list, t_lem *lem)
{
	int l;
	int k;
	int res;

	k = 0;
	l = 0;
	while (list)
	{
		l += path_len(list->path);
		k++;
		list = list->next;
	}
	if (k == 0)
		return (__INT_MAX__);
	res = (lem->ants + l) / k - 1;
	if ((lem->ants + l) % k != 0)
		res += 1;
	return (res);
}

void			error(char *message, t_lem *lem)
{
	char *line;
	char *color;

	if (lem->flag_color)
		color = RED;
	else
		color = DEFAULT;
	while (get_next_line(0, &line) == 1)
	{
		ft_printf("%s%s%s\n", color, line, DEFAULT);
		ft_strdel(&line);
	}
	ft_printf("\n%sERROR line %d: %s%s\n", color,
		get_next_line_counter(GNL_RETURN_COUNT_MODE, 0, NULL, lem),
		message, DEFAULT);
	exit(0);
}

int				main(int argc, char **argv)
{
	t_list_of_pathes	*pathes_1;
	t_list_of_pathes	*pathes_2;
	t_lem				*lem;

	lem = init();
	flags_handle(argc, argv, lem);
	read_map(lem);
	if (lem->start == NULL)
		error("The start room is missing", lem);
	if (lem->end == NULL)
		error("The end room is missing", lem);
	pathes_1 = NULL;
	pathes_2 = NULL;
	get_pathes(&pathes_1, &pathes_2, lem);
	if (total_steps(pathes_1, lem) <= total_steps(pathes_2, lem))
		ants_contribution(pathes_1, lem);
	else
		ants_contribution(pathes_2, lem);
	return (0);
}
