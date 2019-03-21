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
#define ERR_MAGIC_HEADER "Magic header is broken"
#define ERR_NULL_AFTER_NAME "There aren't empty octets next to the name"
#define ERR_NULL_AFTER_COMMENT "There aren't empty octets next to the comment"
#define ERR_BIG_CHAMP "Too big champion"

int		read_bytes(int fd, int amount)
{
	unsigned char buf[amount];
	read(fd, &buf, amount);
	int res = 0;
	int i = -1;
	while (++i < amount)
	{
		if (i != 0)
			res <<= 8;
		res |= buf[i] /*& 0xFF*/;
	}
	return (res);
}

int	read_magic_header(int fd)
{
	int i;
	// int res;

	union magic_header header;
	header.hex = COREWAR_EXEC_MAGIC;

	i = -1;
	while (++i < 4)
	{
		// res = 

		if (read_bytes(fd, 1) != header.bytes[3 - i])
			return (0);
	}
	return (1);
}

void	read_name(int fd, char *name)
{
	int i;
	i = -1;
	int res;
	while (++i < PROG_NAME_LENGTH)
	{
		res = read_bytes(fd, 1);
		if (res != 0)
			name[i] = res;

	}
}

int		read_null(int fd)
{
	int i;
	// int res;

	i = -1;
	while (++i < 4)
	{
		// res = ;
		if (read_bytes(fd, 1) != 0)
			return (0);

	}
	return (1);
}

int		read_exec_code_size(int fd)
{
	union magic_header size;
	int i = -1;
	// int res;
	while (++i < 4)
	{
		// res = 
		size.bytes[3 - i] = read_bytes(fd, 1);
	}
	return (size.hex);
}

void	read_comment(int fd, char *comment)
{
	int i = -1;
	int res;
	while (++i < COMMENT_LENGTH)
	{
		res = read_bytes(fd, 1);
		if (res != 0)
			comment[i] = res;
	}
}

void	read_exec_code(int fd, t_mem_cell *map[], t_champion *champ, int number, int mem_start)
{
	int i = -1;
	int res;
	while (++i < champ->header->prog_size)
	{
		res = read_bytes(fd, 1);
		map[mem_start + i]->value = res;
		map[mem_start + i]->color = number + 1;
	}
}

void	parse_champions(t_champion *champs[], t_mem_cell *map[], int mem_delta)
{
	int i = -1;
	while (champs[++i] != NULL)
	{
		t_champion *champ = champs[i];
		champ->header = ft_memalloc(sizeof(header_t));
		int fd = open(champ->file, O_RDONLY);
		if (fd == -1)
			error(ERR_OPEN_CHAMP);
		if (!read_magic_header(fd)) // to var from header
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
		read_exec_code(fd, map, champ, i, i * mem_delta);
		close(fd);
	}
}