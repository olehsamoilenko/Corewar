/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   champion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

t_champion	*find_champ(int number, t_war *war)
{
	int i = -1;
	while (++i < 4 && war->champs[i])
	{
		if (war->champs[i]->number == -number)
			return (war->champs[i]);
	}
	return (NULL); // mb sf
}

void		read_exec_code(int fd, t_champion *champ, t_war *war)
{
	int i;
	unsigned char tmp;

	int mem_delta = MEM_SIZE / war->amount_champs;
	int mem_start = (champ->number - 1) * mem_delta;


	i = 0;
	while (read(fd, &war->map[mem_start + i]->value, 1))
	{
		war->map[mem_start + i]->color = champ->number - 1 + 1;
		i++;
	}
	if (i != champ->header->prog_size)
		error(ERR_SIZE_DIFFERS);
}

void		parse_champions(t_war *war)
{
	int			fd;
	t_champion	*champ;
	int			i;
	
	i = -1;
	while (war->champs[++i] != NULL)
	{
		champ = war->champs[i];
		champ->header = ft_memalloc(sizeof(header_t));
		if ((fd = open(champ->file, O_RDONLY)) == -1)
			error(ERR_OPEN_CHAMP);
		if (!read_magic_header(fd))
			error(ERR_MAGIC_HEADER);
		read_name(fd, champ->header->prog_name);
		if (!read_null(fd))
			error(ERR_NULL_AFTER_NAME);
		champ->header->prog_size = read_exec_code_size(fd);
		if (champ->header->prog_size > CHAMP_MAX_SIZE)
			error(ERR_BIG_CHAMP);
		read_comment(fd, champ->header->comment);
		if (!read_null(fd))
			error(ERR_NULL_AFTER_COMMENT);
		read_exec_code(fd, champ, war);
		war->last_live = champ;
		close(fd);
	}
}