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

#include <fcntl.h>

#define ERR_MAGIC_HEADER "Magic header is broken"
#define ERR_NULL_AFTER_NAME "There aren't empty octets next to the name"
#define ERR_NULL_AFTER_COMMENT "There aren't empty octets next to the comment"

t_war	*init()
{
	t_war *war = ft_memalloc(sizeof(t_war));
	int i = -1;

	// fake
	while (++i < MEM_SIZE)
	{
		war->map[i] = 0xFF;
	}
	// fake
	return (war);
}

void	print_map(unsigned char *map)
{
	int i = 0;
	while (i < MEM_SIZE)
	{
		printf("%02x%02x ", map[i], map[i + 1]);
		i += 2;
		if (i % 16 == 0)
			printf("\n");
	}
}

void	print_champion(t_champion *champ)
{
	printf("NAME: %s\n", champ->name);
	printf("COMMENT: %s\n", champ->comment);
	printf("SIZE: %d\n", champ->size);
}

void	usage()
{
	printf("Usage: ./vm champion.cor\n");
	exit(0);
}

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

void	fill_map(unsigned char *map, unsigned char value)
{
	static int i = -1;
	map[++i] = value;
	// printf("%i = %02x\n", i, value);
}

void	error(char *message)
{
	printf("Error: %s\n", message);
	exit(0);
}

int	read_magic_header(int fd, unsigned char *map)
{
	int i;
	int res;
	union magic_header header;
	header.hex = COREWAR_EXEC_MAGIC;

	i = -1;
	while (++i < 4)
	{
		res = read_bytes(fd, 1);
		fill_map(map, res);
		if (res != header.bytes[3 - i])
			return (0);
	}
	return (1);
}

void	read_name(int fd, unsigned char *map, char *name)
{
	int i;
	i = -1;
	int res;
	while (++i < PROG_NAME_LENGTH)
	{
		res = read_bytes(fd, 1);
		if (res != 0)
			name[i] = res;
		fill_map(map, res);
	}
}

int		read_null(int fd, unsigned char *map)
{
	int i;
	int res;

	i = -1;
	while (++i < 4)
	{
		res = read_bytes(fd, 1);
		if (res != 0)
			return (0);
		fill_map(map, res);
	}
	return (1);
}

int		read_exec_code_size(int fd, unsigned char *map)
{
	union magic_header size;
	int i = -1;
	int res;
	while (++i < 4)
	{
		res = read_bytes(fd, 1);
		size.bytes[3 - i] = res;
		fill_map(map, res);
	}
	
	return (size.hex);
	
}

void	read_comment(int fd, unsigned char *map, char *comment)
{
	int i = -1;
	int res;
	while (++i < COMMENT_LENGTH)
	{
		res = read_bytes(fd, 1);
		if (res != 0)
		{
			// printf("%i symbol\n", i);
			comment[i] = res;
		}
		fill_map(map, res);
	}
}

void	read_exec_code(int fd, unsigned char *map, int code_size)
{
	int i = -1;
	int res;
	while (++i <= code_size)
	{
		res = read_bytes(fd, 1);
		fill_map(map, res);
	}
}

int		main(int argc, char **argv)
{
	/* TEST */
	// printf("%s\n", header.bytes);
	// printf("%02x %02x %02x %02x\n", header.bytes[0], header.bytes[1], header.bytes[2], header.bytes[3]);
	// if (header.bytes[0] == 0xF3)
	// 	printf("OK!\n");
	/* TEST */

	t_war *war = init();

	

	if (argc == 2)
	{
		int fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			usage();
		
		
		t_champion *zork = ft_memalloc(sizeof(t_champion));
		
		if (!read_magic_header(fd, war->map))
			error(ERR_MAGIC_HEADER);
		read_name(fd, war->map, zork->name);
		if (!read_null(fd, war->map))
			error(ERR_NULL_AFTER_NAME);
		zork->size = read_exec_code_size(fd, war->map);
		read_comment(fd, war->map, zork->comment);
		if (!read_null(fd, war->map))
			error(ERR_NULL_AFTER_COMMENT);
		read_exec_code(fd, war->map, zork->size);
		// change struct to standart

		print_map(war->map);
		print_champion(zork);
	}
	else
		usage();

	return (0);
}
