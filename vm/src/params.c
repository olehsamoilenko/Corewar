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

// args parsing
#define ERR_MANY_CHAMPS "Virtual machine allows up to 4 champions"
#define ERR_NO_CHAMPS "There are any champions present"
#define ERR_N_NUMBER "Flag -n needs a number in range of 1 to 4"
#define ERR_N_CHAMP "Flag -n needs a champion"
#define ERR_SAME_N "Duplication of champion's number is forbidden"
#define ERR_CHAMP_FORMAT "Champion format must be 'name.cor'"
#define ERR_BIG_N "Champion's number exceeds their amount"
#define ERR_DUMP "Flag -dump needs a number"

void	usage()
{
	printf("Usage: ./vm <[-n 1] champion1.cor> <...> [-v] [-visual]\n");
	system("leaks vm | grep 'leaked bytes'");
	exit(0);
}

t_bool		is_champion(char *name) // t_bool
{
	t_bool res;
	char *cor = ft_strsub(name, ft_strlen(name) - 4, 4);
	if (ft_strlen(name) > 4 && ft_strequ(cor, ".cor"))
		res = true;
	else
		res = false;
	ft_strdel(&cor);
	return (res);

}

t_champion	*create_champion(char *file, t_mem_cell *map[])
{
	t_champion *champ = ft_memalloc(sizeof(t_champion));
	champ->file = file;
	return (champ);
}

void	parse_params(int argc, char **argv, t_war *war)
{
	if (argc == 1)
		usage();

	int champs = 0;

	int buf_champs_counter = 0;
	t_champion	**buf_champs = ft_memalloc(4 * sizeof(t_champion));

	int i = 0;
	int num;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-n"))
		{
			if (i >= argc - 1 || !ft_isinteger(argv[i + 1]) || 
				(num = ft_atoi(argv[i + 1])) < 1 || num > MAX_PLAYERS)
				error(ERR_N_NUMBER);

			if (i < argc - 2)
			{
				if (!is_champion(argv[i + 2]))
					error(ERR_CHAMP_FORMAT);
				if (war->champs[num - 1] == NULL)
					war->champs[num - 1] = create_champion(argv[i + 2], war->map);
				else
					error(ERR_SAME_N);
				i += 2;
			}
			else
				error(ERR_N_CHAMP);
		}
		else if (ft_strequ(argv[i], "-v"))
			war->flag_verbose = true;
		else if (ft_strequ(argv[i], "-visual"))
			war->flag_visual = true;
		else if (ft_strequ(argv[i], "-dump"))
		{
			if (i >= argc - 1 || !ft_isinteger(argv[i + 1]) || (num = ft_atoi(argv[i + 1])) < 0)
				error(ERR_DUMP);
			i += 1;
		}
		else if (!is_champion(argv[i]))
			usage();
		else
		{
			buf_champs[buf_champs_counter++] = create_champion(argv[i], war->map);
			if (buf_champs_counter == MAX_PLAYERS)
				error(ERR_MANY_CHAMPS);
		}
	}


	// put buf champs to general
	i = -1;
	while (buf_champs[++i] != NULL)
	{
		int j = -1;
		while (war->champs[++j] != NULL)
		{
			if (j == MAX_PLAYERS - 1)
				error(ERR_MANY_CHAMPS);
		}
		war->champs[j] = buf_champs[i];
	}

	// empty chapms ?
	i = MAX_PLAYERS - 1;
	while (war->champs[i] == NULL)
		i--;
	while (i-- >= 0)
	{
		if (war->champs[i] == NULL)
			error(ERR_BIG_N);
	}
	if (war->champs[0] == NULL)
		error(ERR_NO_CHAMPS);
	free(buf_champs);
}