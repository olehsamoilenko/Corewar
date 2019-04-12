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

// champ parsing
#define ERR_OPEN_CHAMP "Can't open the champion"
#define ERR_MAGIC_HEADER "Magic header is incorrect"
#define ERR_NULL_AFTER_NAME "There aren't empty octets after the name"
#define ERR_NULL_AFTER_COMMENT "There aren't empty octets after the comment"
#define ERR_BIG_CHAMP "Too big champion"
#define ERR_SIZE_DIFFERS "File has a code size that differ from what its header says"
#define ERR_SMALL_FILE "File is too small to be a champion"

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

int	read_magic_header(int fd)
{
	int i;
	t_converter header;
	unsigned char tmp;

	header.integer = COREWAR_EXEC_MAGIC;
	i = -1;
	while (++i < 4)
	{
		read(fd, &tmp, 1);
		if (tmp != header.bytes[3 - i])
			return (0);
	}
	return (1);
}

void	read_name(int fd, char *name)
{
	int i;
	int res;

	i = -1;
	res = 0;
	while (++i < PROG_NAME_LENGTH)
		res += read(fd, &name[i], 1);
	if (res != PROG_NAME_LENGTH)
		error(ERR_SMALL_FILE);
}

int		read_null(int fd)
{
	int i;
	int res;
	unsigned int tmp;

	i = -1;
	res = read(fd, &tmp, 4);
	if (res != 4 || tmp != 0)
		return (0);
	return (1);
}

int		read_exec_code_size(int fd)
{
	t_converter size;
	int i = -1;
	int res;

	res = 0;
	i = -1;
	while (++i < 4)
		res += read(fd, &size.bytes[3 - i], 1);
	// ft_printf("%x\n", size.integer);
	return (size.integer);
}

void	read_comment(int fd, char *comment)
{
	int i = -1;
	int res;

	res = 0;
	while (++i < COMMENT_LENGTH)
		res += read(fd, &comment[i], 1);
	if (res != COMMENT_LENGTH)
		error(ERR_SMALL_FILE);
}

void	read_exec_code(int fd, t_champion *champ, t_war *war)
{
	int i;
	unsigned char tmp;

	int mem_delta = MEM_SIZE / chmps_count(war->champs);
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

void	throw_basic_carriages(t_war *war)
{
	int i;

	i = -1;
	while (war->champs[++i] != NULL)
	{
		t_carriage *car = create_carriage(war, war->champs[i]);
		push_carriage(car, &war->carriages);
	}
}

void	parse_champions(t_war *war)
{
	int fd;
	t_champion *champ;
	int i = -1;
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
	throw_basic_carriages(war);
}