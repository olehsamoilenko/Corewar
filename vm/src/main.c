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



#define KEY_ESC 27
#define KEY_S 115

void	error(char *message) // .h
{
	printf("Error: %s\n", message);
	system("leaks vm | grep 'leaked bytes'");
	exit(0);
}

void	status(char *line, t_war *war)
{
	// wdeleteln(war->win_info);
	mvwaddstr(war->win_info, 4, 2, "                       ");
	mvwaddstr(war->win_info, 4, 2, line);
	wrefresh(war->win_info);
}

void	print_memory(t_war *war)
{
	int i = -1;
	char s[3];

	// war->map[0]->value = 0; // trash oleha
	while (++i < MEM_SIZE)
	{
		// color on
		if (war->map[i]->color == 0)
			wattroff(war->win_mem, A_COLOR);
		else
			wattron(war->win_mem, COLOR_PAIR(war->map[i]->color));

		// bold
		if (war->map[i]->bold == 1)
			wattron(war->win_mem, A_BOLD);
		sprintf(s, "%02x", war->map[i]->value);
		mvwaddstr(war->win_mem, i / 64 + 1, (i % 64) * 3 + 2, s);
		wattroff(war->win_mem, A_BOLD);
	}
	t_carriage *tmp = war->carriages; // need?
	while (tmp)
	{
		wattron(war->win_mem, COLOR_PAIR(4 + tmp->color));
		sprintf(s, "%02x", war->map[tmp->position]->value);
		mvwaddstr(war->win_mem, tmp->position / 64 + 1, (tmp->position % 64) * 3 + 2, s);
		tmp = tmp->next;
	}
	wrefresh(war->win_mem);
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

void	over_over(t_war *war)
{
	delwin(war->win_mem);
	delwin(war->win_info);
	endwin();
	system("leaks vm");
	exit(0);
}

void	over_curses(t_war *war)
{
	int key;
	
	status("overing", war);
	while ((key = wgetch(war->win_getch)) != KEY_ESC)
	{
		// print_memory(war);
		// box(war->win_mem, 0, 0);
		// wrefresh(war->win_mem);
	}
	over_over(war);
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
	car->next = *list;
	*list = car;
}

t_carriage	*create_carriage(int position, int number)
{
	t_carriage *car = ft_memalloc(sizeof(t_carriage));
	car->position = position;
	car->color = number;
	car->reg[1] = -number;
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

int		op_index(int code)
{
	int i = -1;
	while (op_tab[++i].name)
	{
		if (op_tab[i].code == code)
			return (i);
	}
	return (-1);
}


void	curriage_info(t_carriage *car)
{
	printf("INFO pos %d, op %s, cooldown %d\n",
		car->position, op_tab[op_index(car->op_code)].name, car->cooldown);
}

void	reg_info(int *reg, t_war *war)
{
	if (war->flag_verbose)
	{
		int i = -1;
		while (++i <= REG_NUMBER)
			printf("%d ", reg[i]);
		printf("\n");

	}
}

char	*define_arg(int arg_code)
{
	if (arg_code == REG_CODE)
		return("T_REG");
	else if (arg_code == DIR_CODE)
		return("T_DIR");
	else if (arg_code == IND_CODE)
		return ("T_IND");
	else
		return ("---");
}

int		define_size(int arg_code, int label)
{
	if (arg_code == REG_CODE)
		return(1);
	else if (arg_code == DIR_CODE)
	{
		if (label == 0)
			return (4);
		else
			return (2);
	}
	else if (arg_code == IND_CODE)
		return (2);
	else
		return (0);
}

int		get_bytes(int start, int amount, t_mem_cell *map[])
{
	int res = 0;
	int i = -1;
	while (++i < amount)
	{
		if (i != 0)
			res <<= 8;
		res |= map[start + i]->value /*& 0xFF*/;
	}
	return (res);
}

int		get_command(t_carriage *car, t_mem_cell *map[], t_war *war) // returns index
{
	car->op_code = map[car->position]->value;
	int index = op_index(car->op_code);
	if (war->flag_verbose)
	{
		printf("FOUND code %d, index %d, name %s, cooldown %i\n", car->op_code, index, op_tab[index].name, op_tab[index].cooldown);
	}
	car->cooldown = op_tab[index].cooldown;
	status("reading operation", war);
	return (index);
}

void	verbose_ld(int player, int arg_1, int arg_2, t_carriage *car, t_war *war)
{
	if (war->flag_verbose)
	{
		printf("P    %d | ld %d r%d\n", player, arg_1, arg_2);
		printf("ADV 7 (%#06x -> %#06x)\n", car->position, car->position);
	}
	status("doing ld", war);
}

void	verbose_sti(int player, int arg_1, int arg_2, int arg_3, t_carriage *car, t_war *war)
{
	if (war->flag_verbose)
		printf("P    %d | sti r%d %d %d\n", player, arg_1, arg_2, arg_3);
	status("doing sti", war);
}

int	get_args(t_carriage *car, t_mem_cell *map[], int index, t_war *war)
{
	int v[7];

	int delta = 0;
	// car->position++;
	delta++;
	int arg_code = map[car->position + delta]->value;
	// v[0] = arg_code;
	int first = arg_code >> 6;
	int second = arg_code >> 4 & 0b0011;
	int third = arg_code >> 2 & 0b000011;
	if (war->flag_verbose)
		printf("args types %s %s %s\n", define_arg(first), define_arg(second), define_arg(third));
	// if ((first | op_tab[index].args_type[0]) == op_tab[index].args_type[0])
	// 	printf("first ok\n");
	// if ((second | op_tab[index].args_type[1]) == op_tab[index].args_type[1])
	// 	printf("second ok\n");
	// if ((third | op_tab[index].args_type[2]) == op_tab[index].args_type[2])
	// 	printf("third ok\n");
	int arg_1_size = define_size(first, op_tab[index].label);
	int arg_2_size = define_size(second, op_tab[index].label);
	int arg_3_size = define_size(third, op_tab[index].label);
	if (war->flag_verbose)
		printf("args_sizes: %d %d %d\n", arg_1_size, arg_2_size, arg_3_size);
	delta++;
	int arg_1, arg_2, arg_3;
	arg_1 = get_bytes(car->position + delta, arg_1_size, map);
	delta += arg_1_size;
	if (arg_2_size != 0)
	{
		arg_2 = get_bytes(car->position + delta, arg_2_size, map);
		delta += arg_2_size;
	}
	if (arg_3_size != 0)
	{
		arg_3 = get_bytes(car->position + delta, arg_3_size, map);
		delta += arg_3_size;
	}
	if (war->flag_verbose)
		printf("args: %d %d %d\n", arg_1, arg_2, arg_3);

	if (op_tab[index].code == 2) // ld
	{
		car->reg[arg_2] = arg_1; // check < 16
		verbose_ld(1, arg_1, arg_2, car, war);
	}
	else if (op_tab[index].code == 11) // sti
	{
		union magic_header number;
		number.hex = car->reg[arg_1];
		int i = -1;
		while (++i < 4)
		{
			map[car->position + (arg_2 + arg_3) % IDX_MOD + i]->value = number.bytes[i];
			map[car->position + (arg_2 + arg_3) % IDX_MOD + i]->bold = 1;
			// if (war->flag_verbose)
			// 	printf("map[%d] = %x\n", car->position + (arg_2 + arg_3) % IDX_MOD + i, number.bytes[i]);
		}
		verbose_sti(1, arg_1, arg_2, arg_3, car, war);
	}
	return (delta);
}



void	next_cycle(t_war *war, t_carriage *car)
{
	int key = wgetch(war->win_getch);
	// printf("%d\n", key);
	if (key == 27)
		over_over(war);
	// status("cooldown", war);
	if (key == KEY_S)
	{
		war->cycle++;
		car->cooldown--;
		// if (war->flag_visual)
		// {
			// print_memory(war);
			char *itoa = ft_itoa(war->cycle);
			mvwaddstr(war->win_info, 2, 10, itoa);
			ft_strdel(&itoa);
			wrefresh(war->win_info);
		// }
		if (war->flag_verbose)
		{
			printf("It is now cycle %d\n", war->cycle);
			curriage_info(car);
		}
	}
}

void	cooldown(t_war *war)
{
	while (war->carriages->cooldown != 0)
	{
		// status("cooldown", war);
		next_cycle(war, war->carriages);
	}
}

int		main(int argc, char **argv)
{
	

	t_war *war = init();

	int i;
	int index;

	parse_params(argc, argv, war);
	int champ_count = champions_count(war->champs);
	int mem_delta = MEM_SIZE / champ_count;
	parse_champions(war->champs, war->map, mem_delta);

	// print_champions(war->champs);

	throw_basic_carriages(war->champs, &war->carriages, mem_delta);
	// show_carriages(war->carriages);

	// MORTEL

	t_carriage *car = war->carriages;

	if (war->flag_visual)
		init_curses(war);

	// status(ft_itoa(war->carriages->position), war);
	// print_memory(war);
	// wrefresh(war->win_mem);
	print_memory(war);	

	index = get_command(car, war->map, war);
	cooldown(war);
	car->position += get_args(car, war->map, index, war);
	print_memory(war);
	
	index = get_command(car, war->map, war);
	cooldown(war);
	car->position += get_args(car, war->map, index, war);
	print_memory(war);

	index = get_command(car, war->map, war);
	cooldown(war);
	car->position += get_args(car, war->map, index, war);
	print_memory(war);

	index = get_command(car, war->map, war);
	cooldown(war);
	car->position += get_args(car, war->map, index, war);
	print_memory(war);

	index = get_command(car, war->map, war);
	cooldown(war);
	car->position += get_args(car, war->map, index, war);
	print_memory(war);



	if (war->flag_visual)
		over_curses(war);

	system("leaks vm");

	return (0);
}
