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
#define ERR_N_NUMBER "Flag -n needs a number in range of 1 to 4"
#define ERR_N_CHAMP "Flag -n needs a champion"
#define ERR_SAME_N "Duplication of champion's number is forbidden"
#define ERR_CHAMP_FORMAT "Champion format must be 'name.cor'"
#define ERR_BIG_N "Champion's number exceeds their amount"

void	init_curses()
{
	initscr();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	// carriage
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_RED);
	init_pair(8, COLOR_BLACK, COLOR_CYAN);

}

t_war	*init()
{
	t_war *war = ft_memalloc(sizeof(t_war));
	int i = -1;

	
	while (++i < MEM_SIZE)
	{
		war->map[i] = ft_memalloc(sizeof(t_mem_cell));
		// fake
		war->map[i]->value = 0xFF;
	}
	war->carriages = NULL;
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

void	print_champions(t_champion *champs[])
{
	
	int i = -1;
	while (++i < 4)
	{
		printf("\t%i: \n", i + 1);
		if (champs[i] == NULL)
		{
			printf("Empty.\n");
			return ;
		}
		else
		{
			printf("NAME: %s\n", champs[i]->header->prog_name);
			printf("COMMENT: %s\n", champs[i]->header->comment);
			printf("SIZE: %d\n", champs[i]->header->prog_size);
		}
	}

	// t_carriage *list = NULL;
	// printf("File: %s\n", champ->file);
	
}

void	usage()
{
	printf("Usage: ./vm <[-n 1] champion1.cor> <...>\n");
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

t_champion	*create_champion(char *file, t_mem_cell *map[])
{
	t_champion *champ = ft_memalloc(sizeof(t_champion));
	champ->file = file;
	return (champ);
}

void	print_memory(t_mem_cell *map[], t_carriage *carriages)
{
	int i = -1;
	char s[3];

	while (++i < MEM_SIZE)
	{
		if (map[i]->color == 0)
			attroff(A_COLOR);
		else
			attron(COLOR_PAIR(map[i]->color));
		sprintf(s, "%02x", map[i]->value);
		mvaddstr(i / 64, (i % 64) * 3, s);
	}
	while (carriages)
	{
		attron(COLOR_PAIR(4 + carriages->color));
		sprintf(s, "%02x", map[carriages->position]->value);
		mvaddstr(carriages->position / 64, (carriages->position % 64) * 3, s);
		carriages = carriages->next;
	}
}

// void	check_mem(unsigned char *map)
// {
// 	int i = -1;
// 	while (++i < MEM_SIZE)
// 	{
// 		if (i % 64 == 0)
// 			printf("\n");
// 		printf("%02x ", map[i]);
		
// 	}
// }

// void	check_color(int *map_color)
// {
// 	int i = -1;
// 	while (++i < MEM_SIZE)
// 	{
// 		if (i % 64 == 0)
// 			printf("\n");
// 		printf("%d ", map_color[i]);
		
// 	}
// }

int		is_champion(char *name)
{
	int res;
	char *cor = ft_strsub(name, ft_strlen(name) - 4, 4);
	if (ft_strlen(name) > 4 && ft_strequ(cor, ".cor"))
		res = 1;
	else
		res = 0;
	ft_strdel(&cor);
	return (res);

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
	free(buf_champs);
}

void	over_curses()
{
	getch();
	endwin();
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

int		champions_count(t_champion **champs)
{
	int count = 0;
	int i = -1;
	while (champs[++i] != NULL)
		count++;
	return (count);
}


void	push_carriage(t_carriage *car, t_carriage **list)
{
	// if (*list == NULL)
	// {
	// 	*list = car;
	// 	return ;
	// }
	// t_carriage *tmp = *list;
	// while (tmp->next)
	// 	tmp = tmp->next;
	car->next = *list;
	// tmp->next = car;
	*list = car;
}

t_carriage	*create_carriage(int position, int color)
{
	t_carriage *car = ft_memalloc(sizeof(t_carriage));
	car->position = position;
	car->color = color;
	return (car);
}

void		show_carriages(t_carriage *list)
{
	while (list)
	{
		printf("Position: %d\tColor: %d\n", list->position, list->color);
		list = list->next;
	}
}

void		throw_basic_carriages(t_champion *champs[], t_carriage **carriages, int mem_delta)
{
	int i = -1;
	while (champs[++i] != NULL)
	{
		push_carriage(create_carriage(mem_delta * i, i + 1), carriages);
	}
}



int		main(int argc, char **argv)
{
	/* TEST */

	// char *line = "123";
	// printf("|%s|\n", ft_strsub(line, 3, 1));

	/* TEST */

	t_war *war = init();

	int i;

	parse_params(argc, argv, war);
	int champ_count = champions_count(war->champs);
	int mem_delta = MEM_SIZE / champ_count;
	parse_champions(war->champs, war->map, mem_delta);

	// print_champions(war->champs);

	throw_basic_carriages(war->champs, &war->carriages, mem_delta);
	show_carriages(war->carriages);
	
	init_curses();
	print_memory(war->map, war->carriages);
	over_curses();

	// check_color(war->map_color);
	system("leaks vm");

	return (0);
}
