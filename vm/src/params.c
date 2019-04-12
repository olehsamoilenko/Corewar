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

#define ERR_MANY_CHAMPS "Virtual machine allows up to 4 champions"
#define ERR_NO_CHAMPS "There are any champions present"
#define ERR_N_NUMBER "Flag -n needs a number in range of 1 to 4"
#define ERR_N_CHAMP "Flag -n needs a champion"
#define ERR_SAME_N "Duplication of champion's number is forbidden"
#define ERR_CHAMP_FORMAT "Champion format must be 'name.cor'"
#define ERR_BIG_N "Champion's number exceeds their amount"
#define ERR_DUMP "Flag -dump needs a number"
#define ERR_VER_VIS "Combination of -verbose and -visual is forbidden"

void	usage(void)
{
	ft_printf("Usage: ./corewar <[-n number] champion1.cor> <...> [-verbose] \
									[-visual] [-dev] [-dump cycle_number]\n");
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

t_bool		is_champion(char *name)
{
	t_bool	res;
	char	*cor;

	cor = ft_strsub(name, ft_strlen(name) - 4, 4);
	if (ft_strlen(name) > 4 && ft_strequ(cor, ".cor"))
		res = true;
	else
		res = false;
	ft_strdel(&cor);
	return (res);
}

t_champion	*create_champion(char *file)
{
	t_champion *champ;

	champ = ft_memalloc(sizeof(t_champion));
	champ->file = file;
	return (champ);
}

void	check_champs_number(t_war *war, int cur_champs)
{
	int		i;
	int		zero_count;
	t_bool	nums[4];
	int		j;

	i = -1;
	if (war->champs[0] == NULL)
		error(ERR_NO_CHAMPS);
	ft_bzero(nums, 16);
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
}

void	get_n_args(int argc, char **argv, t_war *war, int *cur_champs, int *i)
{
	int n;

	if (*i >= argc - 1 || !ft_isinteger(argv[*i + 1]) ||
		(n = ft_atoi(argv[*i + 1])) < 1 || n > MAX_PLAYERS)
		error(ERR_N_NUMBER);
	if (*i < argc - 2)
	{
		if (!is_champion(argv[*i + 2]))
			error(ERR_CHAMP_FORMAT);
		if (*cur_champs == 4)
			error(ERR_MANY_CHAMPS);
		war->champs[*cur_champs] = create_champion(argv[*i + 2]);
		war->champs[*cur_champs]->number = n;
		*cur_champs += 1;
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
	int cur_champs;
	int i;

	cur_champs = 0;
	i = 0;
	if (argc == 1)
		usage();
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-n"))
			get_n_args(argc, argv, war, &cur_champs, &i);
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
			if (cur_champs == 4)
				error(ERR_MANY_CHAMPS);
			war->champs[cur_champs++] = create_champion(argv[i]);
		}
	}
	check_champs_number(war, cur_champs);
}
