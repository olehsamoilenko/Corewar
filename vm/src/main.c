/*                                                                            */
/* ************************************************************************** */
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

void	error(char *message) // .h
{
	ft_printf("Error: %s\n", message);
	system("leaks vm | grep 'leaked bytes'");
	exit(0);
}

void	next_cycle(t_war *war)
{
	
	int key = 0;
	if (war->flag_visual && war->cycle >= war->flag_dump)
	{
		key = wgetch(war->win_getch);
		if (key == 27)
			over_over(war);
	}
	if (key == KEY_S || war->cycle < war->flag_dump || !war->flag_visual)
	{
		war->cycle++;
		// car->cooldown--;
	}
	if (war->flag_visual && (key == KEY_S || war->cycle == war->flag_dump))
	{
		// print_info(war);
		print_memory(war);
	}
	
	
	
	if (war->flag_verbose)
	{
		ft_printf("It is now cycle %d\n", war->cycle);
		// curriage_info(car);
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

void		throw_basic_carriages(t_champion *champs[], t_carriage **carriages, int mem_delta, t_war *war)
{
	int i = -1;
	while (champs[++i] != NULL)
	{
		push_carriage(create_carriage(mem_delta * i, i + 1, war, champs[i]), carriages);
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

int		define_size(int arg_code, int label)
{
	if (arg_code == REG_CODE)
		return(1);
	else if (arg_code == DIR_CODE)
	{
		if (label == false)
			return (4);
		else
			return (2);
	}
	else if (arg_code == IND_CODE)
		return (2);
	else
		return (0);
}

int		get_bytes(int start, int amount, int type, t_mem_cell *map[])
{
	int res = 0;
	int i = -1;
	while (++i < amount)
	{
		if (i != 0)
			res <<= 8;
		res |= map[start + i]->value /*& 0xFF*/;
		// ft_printf("%x\n", res);
	}
	// ft_printf("\n");
	if (amount == 2)
	{
		return ((short)res);
	}

	return (res); // to unsigned ?
}

t_op		*get_command(int process, int car_pos, t_mem_cell *map[], t_war *war) // returns index
{
	int index = op_index(map[car_pos]->value); // return op ?
	// if (index == -1)
	// {
	// 	ft_printf("UNKNOWN COMMAND: %02x\n", map[car_pos]->value);
	// 	ft_printf("Bratik, realizuy pls\n");
	// 	exit(0);
	// }
	t_op *op = &op_tab[index];
	if (war->flag_verbose)
		ft_printf("Process %d FOUND code %d, index %d, name %s, cooldown %i\n", process, op->code, index, op->name, op_tab[index].cooldown);
	return (op);
}

t_instr_params	*get_args(t_carriage *car, t_mem_cell *map[], t_op *op, t_war *war)
{
	int v[7];

	t_instr_params *params = ft_memalloc(sizeof(t_instr_params));

	int delta = 0;
	// car->position++;
	delta++;
	int first;
	int second;
	int third;
	int codage;
	params->amount = op->args;
	if (op->codage == true)
	{
		codage = map[car->position + delta]->value;
		params->codage = codage;
		first = codage >> 6;
		second = codage >> 4 & 0b0011;
		third = codage >> 2 & 0b000011;
		delta++;
	}
	else
	{
		first = op->args_type[0];
		second = op->args_type[1];
		third = op->args_type[2];
	}

	params->types[1] = first;
	params->types[2] = second;
	params->types[3] = third;

	int arg_1_size = define_size(first, op->label);
	int arg_2_size = define_size(second, op->label);
	int arg_3_size = define_size(third, op->label);

	params->sizes[1] = arg_1_size;
	params->sizes[2] = arg_2_size;
	params->sizes[3] = arg_3_size;

	
	// if (war->flag_verbose)
	// 	ft_printf("args_sizes: %d %d %d\n", arg_1_size, arg_2_size, arg_3_size);
	


	unsigned int arg_1 = 0, arg_2 = 0, arg_3 = 0;
	arg_1 = get_bytes(car->position + delta, arg_1_size, params->types[1], map);
	delta += arg_1_size;
	if (arg_2_size != 0)
	{
		arg_2 = get_bytes(car->position + delta, arg_2_size, params->types[2], map);
		delta += arg_2_size;
	}
	if (arg_3_size != 0)
	{
		arg_3 = get_bytes(car->position + delta, arg_3_size, params->types[3], map);
		delta += arg_3_size;
	}

	params->params[1].integer = arg_1;
	params->params[2].integer = arg_2;
	params->params[3].integer = arg_3;

	return (params);
}

void	introduce(t_champion **champs)
{
	ft_printf("Introducing contestants...\n");
	int i = -1;
	while (champs[++i])
	{
		ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
			champs[i]->number, champs[i]->header->prog_size,
			champs[i]->header->prog_name, champs[i]->header->comment);
	}
}

int		main(int argc, char **argv)
{
	t_war *war = init();

	int index;

	parse_params(argc, argv, war);
	int mem_delta = MEM_SIZE / champions_count(war->champs);
	parse_champions(war->champs, war->map, mem_delta);
	// print_champions(war->champs);
	if (!war->flag_visual)
		introduce(war->champs);

	throw_basic_carriages(war->champs, &war->carriages, mem_delta, war);

	if (war->flag_visual)
		init_curses(war);
	if (war->cycle >= war->flag_dump)
		print_memory(war);
	if (!war->flag_visual && war->cycle == war->flag_dump)
		dump(war); // dump 0

	for (int i = 0; i < 1160; i++)
	{
		t_carriage *tmp = war->carriages;
		next_cycle(war);
		
		while (tmp)
		{
			// MORTEL

			t_carriage *car = tmp;
			if (car->cooldown == 0) // new process or forked
			{
				car->op = get_command(car->number, car->position, war->map, war);
				car->cooldown = car->op->cooldown;
			}
			car->cooldown--;
			if (car->cooldown == 0)
			{
				t_instr_params *params = get_args(car, war->map, car->op, war);
				// show_args(params);

				car->op->func(car, war, params);
				int instr_len = 1 + car->op->codage + params->sizes[1] + params->sizes[2] + params->sizes[3];
				if (car->op->code != 0x09) // zjmp
				{
					adv(war, car->op, instr_len, car, params);
					car->position += instr_len;
				}
				free(params);
				
				print_memory(war);
				car->op = get_command(car->number, car->position, war->map, war);
				car->cooldown = car->op->cooldown;
			}
			tmp = tmp->next;
		}
		if (!war->flag_visual && war->cycle == war->flag_dump)
			dump(war);
		
	}
	
	if (war->flag_visual)
		over_curses(war);
	system("leaks vm");
	return (0);
}
