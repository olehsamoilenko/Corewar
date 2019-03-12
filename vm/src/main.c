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
#include <curses.h>

// champ parsing
#define ERR_MAGIC_HEADER "Magic header is broken"
#define ERR_NULL_AFTER_NAME "There aren't empty octets next to the name"
#define ERR_NULL_AFTER_COMMENT "There aren't empty octets next to the comment"
#define ERR_BIG_CHAMP "Too big champion"

// args parsing
#define ERR_MANY_CHAMPS "Virtual machine allows up to 4 champions"
#define ERR_OPEN_CHAMP "Can't open the champion"

void	init_curses()
{
	initscr();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);

}

t_war	*init()
{
	t_war *war = ft_memalloc(sizeof(t_war));
	int i = -1;

	// fake
	while (++i < MEM_SIZE)
		war->map[i] = 0xFF;
	init_curses();

	return (war);
}

void	print_champion_hex(unsigned char *map)
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
	printf("NAME: %s\n", champ->header->prog_name);
	printf("COMMENT: %s\n", champ->header->comment);
	printf("SIZE: %d\n", champ->header->prog_size);
}

void	usage()
{
	printf("Usage: ./vm <champion1.cor> <...>\n");
	system("leaks vm | grep 'leaked bytes'");
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

void	error(char *message)
{
	printf("Error: %s\n", message);
	system("leaks vm | grep 'leaked bytes'");
	exit(0);
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

void	read_exec_code(int fd, unsigned char *map, int *map_color, int champ_number, int mem_start, int code_size)
{
	int i = -1;
	int res;
	while (++i < code_size)
	{
		res = read_bytes(fd, 1);
		map[mem_start + i] = res;
		map_color[mem_start + i] = champ_number;
	}
}

t_champion	*create_champion(char *file, unsigned char *map, int *map_color, int champ_number, int mem_start)
{
	t_champion *champ = ft_memalloc(sizeof(t_champion));
	champ->header = ft_memalloc(sizeof(header_t));

	int mem_num = mem_start;

	int fd = open(file, O_RDONLY);
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
	read_exec_code(fd, map, map_color, champ_number, mem_start, champ->header->prog_size);

	close(fd);

	return (champ);
}

void	print_memory(unsigned char *map, int *map_color)
{
	int i = -1;
	char s[3];
	// attron(COLOR_PAIR(1));
	while (++i < MEM_SIZE)
	{
		if (map_color[i] == 0)
			attroff(A_COLOR);
		else
			attron(COLOR_PAIR(map_color[i]));
		sprintf(s, "%02x", map[i]);
		mvaddstr(i / 64, (i % 64) * 3, s);
	}
	// attroff(COLOR_PAIR(1));
}

void	check_mem(unsigned char *map)
{
	int i = -1;
	while (++i < MEM_SIZE)
	{
		if (i % 64 == 0)
			printf("\n");
		printf("%02x ", map[i]);
		
	}
}

void	check_color(int *map_color)
{
	int i = -1;
	while (++i < MEM_SIZE)
	{
		if (i % 64 == 0)
			printf("\n");
		printf("%d ", map_color[i]);
		
	}
}

void	over_curses()
{
	getch();
	endwin();
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

	if (argc == 1)
		usage();
	if (argc > 5)
		error(ERR_MANY_CHAMPS);
	
	int mem_delta = MEM_SIZE / (argc - 1);
	int i = -1;
	while (++i < argc - 1)
		war->champs[i] = create_champion(argv[i + 1], war->map, war->map_color, i + 1, i * mem_delta);

	
	print_memory(war->map, war->map_color);

	over_curses();
	

	// check_color(war->map_color);
	system("leaks vm");
	return (0);
}
