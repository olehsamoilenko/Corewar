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
#define ERR_LOST_SIZE "Size of the champion is missed"

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

int		read_bytes(int fd, int amount, int *bytes)
{
	unsigned char buf[amount];
	*bytes += read(fd, &buf, amount);

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

	union converter header;
	header.integer = COREWAR_EXEC_MAGIC;
	int bytes;

	i = -1;
	while (++i < 4)
	{
		if (read_bytes(fd, 1, &bytes) != header.bytes[3 - i])
			return (0);
	}
	return (1);
}

void	read_name(int fd, char *name)
{
	int i;
	i = -1;
	int res;
	int bytes;

	bytes = 0;
	while (++i < PROG_NAME_LENGTH)
	{
		res = read_bytes(fd, 1, &bytes);
		if (res != 0)
			name[i] = res;

	}
	// printf("%d\n", bytes);
	if (bytes != PROG_NAME_LENGTH)
		error(ERR_SMALL_FILE);
}

int		read_null(int fd)
{
	int i;
	// int res;
	int bytes;

	i = -1;
	bytes = 0;
	while (++i < 4)
	{
		// res = ;
		if (read_bytes(fd, 1, &bytes) != 0)
			return (0);

	}
	if (bytes == 4)
		return (1);
	return (0);
}

int		read_exec_code_size(int fd)
{
	union converter size;
	int i = -1;
	int bytes;
	// int res;
	bytes = 0;
	while (++i < 4)
	{
		// res = 
		size.bytes[3 - i] = read_bytes(fd, 1, &bytes);
	}
	return (size.integer);
}

void	read_comment(int fd, char *comment)
{
	int i = -1;
	int res;
	int bytes;

	bytes = 0;
	while (++i < COMMENT_LENGTH)
	{
		res = read_bytes(fd, 1, &bytes);
		if (res != 0)
			comment[i] = res;
	}
	if (bytes != COMMENT_LENGTH)
		error(ERR_SMALL_FILE);
}

int 	read_exec_code(int fd, t_mem_cell *map[], t_champion *champ, int number, int mem_start)
{
	int i = -1;
	int res;
	int bytes;

	bytes = 0;
	// цикл всегда будет идти до prog_size, но код чемпиона может быть больше чем размер,
	// который уже считали после имени
	while (++i < champ->header->prog_size)
	{
		res = read_bytes(fd, 1, &bytes);
		map[mem_start + i]->value = res;
		map[mem_start + i]->color = number + 1;
	}
	printf("%d\n", bytes);
	return (bytes);
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
		if (read_exec_code(fd, map, champ, i, i * mem_delta) != champ->header->prog_size)
			error(ERR_SIZE_DIFFERS);
		// read_exec_code(fd, map, champ, i, i * mem_delta);
		close(fd);
	}
}