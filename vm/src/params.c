/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"



void	usage(void)
{
	ft_printf("Usage: ./corewar <[-n number] champion1.cor> <...> [-verbose] \
									[-visual] [-dev] [-dump cycle_number]\n");
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

t_bool	*check_errors(t_war *war, int cur_champs)
{
	int		i;
	int		zero_count;
	t_bool	*nums;

	i = -1;
	nums = ft_memalloc(sizeof(t_bool) * 4);
	zero_count = 0;
	while (++i < cur_champs)
	{
		if (war->champs[i]->number == 0)
			zero_count++;
		else if (war->champs[i]->number > cur_champs)
			error(ERR_BIG_N);
		else if (nums[war->champs[i]->number - 1] == true)
			error(ERR_SAME_N);
		else
			nums[war->champs[i]->number - 1] = true;
	}
	return (nums);
}

void	check_champs_number(t_war *war, int cur_champs)
{
	int		i;
	t_bool	*nums;
	int		j;

	if (war->champs[0] == NULL)
		error(ERR_NO_CHAMPS);
	nums = check_errors(war, cur_champs);
	i = -1;
	while (++i < cur_champs)
	{
		if (war->champs[i]->number == 0)
		{
			j = 0;
			while (nums[j] != false)
				j++;
			war->champs[i]->number = j + 1;
			nums[j] = true;
		}
	}
	if (war->flag_verbose && war->flag_visual)
		error(ERR_VER_VIS);
	free(nums);
}

void	get_n_args(int argc, char **argv, t_war *war, int *i)
{
	int n;

	if (*i >= argc - 1 || !ft_isinteger(argv[*i + 1]) ||
		(n = ft_atoi(argv[*i + 1])) < 1 || n > MAX_PLAYERS)
		error(ERR_N_NUMBER);
	if (*i < argc - 2)
	{
		if (!is_champion(argv[*i + 2]))
			error(ERR_CHAMP_FORMAT);
		if (war->amount_champs == 4)
			error(ERR_MANY_CHAMPS);
		war->champs[war->amount_champs] = create_champion(argv[*i + 2]);
		war->champs[war->amount_champs]->number = n;
		war->amount_champs += 1;
		*i += 2;
	}
	else
		error(ERR_N_CHAMP);
}

void	get_dump(int argc, char **argv, t_war *war, int *i)
{
	int n;

	if (*i >= argc - 1 || !ft_isinteger(argv[*i + 1]) ||
							(n = ft_atoi(argv[*i + 1])) < 0)
		error(ERR_DUMP);
	war->flag_dump = ft_atoi(argv[*i + 1]);
	*i += 1;
}

void	parse_params(int argc, char **argv, t_war *war)
{
	int i;

	i = 0;
	if (argc == 1)
		usage();
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-n"))
			get_n_args(argc, argv, war, &i);
		else if (ft_strequ(argv[i], "-verbose"))
			war->flag_verbose = true;
		else if (ft_strequ(argv[i], "-visual"))
			war->flag_visual = true;
		else if (ft_strequ(argv[i], "-dump"))
			get_dump(argc, argv, war, &i);
		else if (!is_champion(argv[i]))
			usage();
		else
		{
			if (war->amount_champs == 4)
				error(ERR_MANY_CHAMPS);
			war->champs[war->amount_champs++] = create_champion(argv[i]);
		}
	}
	check_champs_number(war, war->amount_champs);
}
