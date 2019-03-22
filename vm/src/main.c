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
	ft_printf("Error: %s\n", message);
	system("leaks vm | grep 'leaked bytes'");
	exit(0);
}

void	status(char *line, t_war *war)
{
	if (war->flag_visual)
	{
		mvwaddstr(war->win_info, 4, 2, "                       ");
		mvwaddstr(war->win_info, 4, 2, line);
		wrefresh(war->win_info);
	}
}

void	curriage_info(t_carriage *car)
{
	// ft_printf("INFO pos %d, op %s, cooldown %d\n",
	// 	car->position, op_tab[op_index(car->op_code)].name, car->cooldown);
}

void	over_over(t_war *war)
{
	delwin(war->win_mem);
	delwin(war->win_info);
	endwin();
	system("leaks vm");
	exit(0);
}

void	print_memory(t_war *war)
{
	int i = -1;
	char s[3];

	while (++i < MEM_SIZE)
	{
		// color on
		if (war->map[i]->color == 0)
			wattroff(war->win_mem, A_COLOR);
		else
			wattron(war->win_mem, COLOR_PAIR(war->map[i]->color));

		// bold
		if (war->map[i]->cycles_bold && war->cycle - war->map[i]->cycles_bold < 50 - 1) 
			wattron(war->win_mem, A_BOLD);

		// live
		if (war->map[i]->cycles_live && war->cycle - war->map[i]->cycles_live < 50 - 1) // 50 ?
		{
			wattron(war->win_mem, A_BOLD);
			wattron(war->win_mem, COLOR_PAIR(8 + war->map[i]->color));
		}


		sprintf(s, "%02x", war->map[i]->value);
		mvwaddstr(war->win_mem, i / 64 + 1, (i % 64) * 3 + 2, s);
		wattroff(war->win_mem, A_BOLD);
	}
	t_carriage *tmp = war->carriages; // need?
	while (tmp)
	{
		wattron(war->win_mem, COLOR_PAIR(4 + war->map[tmp->position]->color)); // number
		sprintf(s, "%02x", war->map[tmp->position]->value);
		mvwaddstr(war->win_mem, tmp->position / 64 + 1, (tmp->position % 64) * 3 + 2, s);
		tmp = tmp->next;
	}
	wrefresh(war->win_mem);

	char *itoa = ft_itoa(war->cycle);
	mvwaddstr(war->win_info, 2, 10, itoa);
	ft_strdel(&itoa);
	wrefresh(war->win_info);
}

// void	print_info(t_war *war)
// {

// }

void	next_cycle(t_war *war, t_carriage *car)
{
	
	int key = 0;
	if (war->flag_visual)
	{
		key = wgetch(war->win_getch);
		if (key == 27)
			over_over(war);
	}
	if (key == KEY_S || !war->flag_visual)
	{
		war->cycle++;
		car->cooldown--;
	}
	if (war->flag_visual && key == KEY_S)
	{
		// print_info(war);
		print_memory(war);
	}
	
	
	if (war->flag_verbose)
	{
		ft_printf("It is now cycle %d\n", war->cycle);
		curriage_info(car);
	}
}

void	print_champion_hex(unsigned char *map)
{
	int i = 0;
	while (i < MEM_SIZE)
	{
		ft_printf("%02x%02x ", map[i], map[i + 1]);
		i += 2;
		if (i % 16 == 0)
			ft_printf("\n");
	}
}

void	print_champions(t_champion *champs[])
{
	
	int i = -1;
	while (++i < 4)
	{
		// ft_printf("\t%i: \n", i + 1);
		if (champs[i] == NULL)
		{
			ft_printf("Empty.\n");
			// return ;
		}
		else
		{
			ft_printf("n = %d, NAME: %s\n", champs[i]->number, champs[i]->header->prog_name);
			// ft_printf("COMMENT: %s\n", champs[i]->header->comment);
			// ft_printf("SIZE: %d\n", champs[i]->header->prog_size);
		}
	}
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

t_carriage	*create_carriage(int position, int number, t_war *war)
{
	t_carriage *car = ft_memalloc(sizeof(t_carriage));
	car->position = position;
	car->player = war->champs[number - 1];
	car->reg[1] = -number;
	return (car);
}

void		show_carriages(t_carriage *list)
{
	while (list)
	{
		ft_printf("Position: %d\tPlayer: %d\n", list->position, list->player);
		list = list->next;
	}
}

void		throw_basic_carriages(t_champion *champs[], t_carriage **carriages, int mem_delta, t_war *war)
{
	int i = -1;
	while (champs[++i] != NULL)
	{
		push_carriage(create_carriage(mem_delta * i, i + 1, war), carriages);
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




void	reg_info(int *reg, t_war *war)
{
	if (war->flag_verbose)
	{
		int i = -1;
		while (++i <= REG_NUMBER)
			ft_printf("%d ", reg[i]);
		ft_printf("\n");

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
		// printf("%x\n", res);
	}
	return (res); // to unsigned ?
}


int		get_command(t_carriage *car, t_mem_cell *map[], t_war *war) // returns index
{
	car->op_code = map[car->position]->value;
	int index = op_index(car->op_code);
	if (war->flag_verbose)
	{
		ft_printf("FOUND code %d, index %d, name %s, cooldown %i\n", car->op_code, index, op_tab[index].name, op_tab[index].cooldown);
	}
	car->cooldown = op_tab[index].cooldown;
	status("reading operation", war);
	return (index);
}







int	get_args(t_carriage *car, t_mem_cell *map[], int index, t_war *war, int *arg)
{
	int v[7];

	int delta = 0;
	// car->position++;
	delta++;
	int first;
	int second;
	int third;
	if (op_tab[index].codage == true)
	{
		if (war->flag_verbose)
			ft_printf("codage on\n");
		int arg_code = map[car->position + delta]->value;
		if (war->flag_verbose)
			ft_printf("args code: %d\n", arg_code);

		first = arg_code >> 6;
		second = arg_code >> 4 & 0b0011;
		third = arg_code >> 2 & 0b000011;
		// check args types
		if (war->flag_verbose)
		{
			ft_printf("args types %s %s %s\n", define_arg(first), define_arg(second), define_arg(third));
			if ((first | op_tab[index].args_type[0]) == op_tab[index].args_type[0])
				ft_printf("first ok\n");
			else
				ft_printf("first KO!\n");
			if ((second | op_tab[index].args_type[1]) == op_tab[index].args_type[1])
				ft_printf("second ok\n");
			else
				ft_printf("second KO!\n");
			if ((third | op_tab[index].args_type[2]) == op_tab[index].args_type[2])
				ft_printf("third ok\n");
			else
				ft_printf("third KO!\n");
		}
		// check args types
		delta++;
	}
	else
	{
		if (war->flag_verbose)
			ft_printf("codage off\n");
		first = op_tab[index].args_type[0];
		second = op_tab[index].args_type[1];
		third = op_tab[index].args_type[2];
	}

	int arg_1_size = define_size(first, op_tab[index].label);
	int arg_2_size = define_size(second, op_tab[index].label);
	int arg_3_size = define_size(third, op_tab[index].label);
	if (war->flag_verbose)
		ft_printf("args_sizes: %d %d %d\n", arg_1_size, arg_2_size, arg_3_size);
	
	int arg_1 = 0, arg_2 = 0, arg_3 = 0;
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
		ft_printf("args: %d %d %d\n", arg_1, arg_2, arg_3);

	arg[1] = arg_1;
	arg[2] = arg_2;
	arg[3] = arg_3;
	return (delta);
}

void	dump(t_war *war)
{
	int i = -1;

	// introducing

	while (++i < MEM_SIZE)
	{
		if (i % 64 == 0)
		{
			if (i == 0)
				ft_printf("0x0000 : ");
			else
				ft_printf("\n%#06x : ", i);
		}
			
		ft_printf("%02x ", war->map[i]->value);
	}
	ft_printf("\n");
}

void	cooldown(t_war *war)
{
	while (war->carriages->cooldown != 0)
	{
		next_cycle(war, war->carriages);
		if (war->cycle == war->flag_dump) // work ?
		{
			dump(war);
		}
	}
}

void	operation(int index, t_carriage *car, t_war *war, int *arg)
{
	if (op_tab[index].code == 2) // ld
	{
		
	}
	else if (op_tab[index].code == 11) // sti
	{
		
	}
	else if (op_tab[index].code == 1) // live
	{
		
	}
}

int		main(int argc, char **argv)
{
	

	t_war *war = init();

	int index;

	parse_params(argc, argv, war);
	int champ_count = champions_count(war->champs);
	int mem_delta = MEM_SIZE / champ_count;
	parse_champions(war->champs, war->map, mem_delta);
	print_champions(war->champs);

	throw_basic_carriages(war->champs, &war->carriages, mem_delta, war);

	// MORTEL

	t_carriage *car = war->carriages;



	if (war->flag_visual)
		init_curses(war);

	// print_memory(war);
	// next_cycle(war, car);
	// print_info(war);

	print_memory(war);
	// cooldown(war);


	for(int i = 0; i < 10; i++)
	{
		index = get_command(car, war->map, war); // ld
		cooldown(war);
		int arg[4] = {0, 0, 0, 0}; // arg[0] is unused
		int delta = get_args(car, war->map, index, war, arg);
		// printf("%d\n", arg[0]);
		op_tab[index].func(index, car, war, arg);
		// operation(index, car, war, arg);
		car->position += delta;
		print_memory(war);
	}
	



	if (war->flag_visual)
		over_curses(war);



	system("leaks vm");

	return (0);
}
