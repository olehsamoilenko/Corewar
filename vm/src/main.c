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
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(message, 2);
	system("leaks corewar | grep 'leaked bytes'");
	exit(0);
}

void	handle_key(int key, t_war *war)
{
	if (key == KEY_ESC)
		over_over(war);
	if (key == KEY_W)
	{
		if (war->cycles_in_second > 1)
			war->cycles_in_second -= 1;			
	}
	if (key == KEY_E)
		war->cycles_in_second += 1;
	if (key == KEY_SPACE)
		war->flag_run = (war->flag_run == true) ? false : true;
}

t_bool	next_cycle(t_war *war)
{
	int		key;
	t_bool	success;

	success = false;
	if (war->flag_visual && war->cycle >= war->flag_dump)
	{
		handle_key(key = getch(), war);
		print_info(war);
	}
	if (!war->flag_visual || key == KEY_S || war->cycle < war->flag_dump ||
	(war->flag_run && ((float)clock() / CLOCKS_PER_SEC - war->time >= 1.0 / war->cycles_in_second)))
	{
		war->cycle += 1;
		success = true;
		if (war->flag_visual && (!war->flag_run || war->cycle - war->last_print >= war->cycles_in_second))
		{
			war->time = (float)clock() / CLOCKS_PER_SEC;
			war->last_print = war->cycle;
		}
		war->time += 1.0 / war->cycles_in_second;
	}
	if (war->flag_visual)
	{
		if (war->cycle >= war->flag_dump)
			print_memory(war);
		print_info(war);
	}
	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("It is now cycle %d\n", war->cycle);
	return (success);
}


int		chmps_count(t_champion **champs)
{
	int count = 0;
	int i = -1;
	while (champs[++i] != NULL)
		count++;
	return (count);
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

int		op_index(int code)
{
	int i = -1;
	while (g_op_tab[++i].name)
	{
		if (g_op_tab[i].code == code)
			return (i);
	}
	return (-1);
}

int		define_size(int arg_code, int label)
{
	if (arg_code == T_REG)
		return (1);
	else if (arg_code == T_DIR)
	{
		if (label == false)
			return (4);
		else
			return (2);
	}
	else if (arg_code == T_IND)
		return (2);
	else
		return (0);
}

int		get_bytes(int start, int amount, int type, t_map_cell *map[])
{
	int res;
	int i;

	res = 0;
	i = -1;
	while (++i < amount)
	{
		if (i != 0)
			res <<= 8;
		res |= map[(start + i) % MEM_SIZE]->value;
	}
	if (amount == 2)
		return ((short)res);
	else
		return (res);
}

t_op		*get_command(int process, int car_pos, t_map_cell **map, t_war *war) // returns index
{
	int index = op_index(map[car_pos]->value); // return op ?
	t_op *op = &g_op_tab[index];
	if (index == -1)
		return (NULL);
	return (op);
}

int		define_type(int type)
{
	if (type == REG_CODE)
		return(T_REG);
	else if (type == DIR_CODE)
		return(T_DIR);
	else if (type == IND_CODE)
		return (T_IND);
	else
		return (0b111);
}

int		get_args(t_carriage *car, t_map_cell **map, t_op *op, t_war *war)
{
	if (op->codage == true)
	{
		car->codage = map[(car->position + 1) % MEM_SIZE]->value;
		car->types[1] = define_type(car->codage >> 6);
		car->types[2] = define_type(car->codage >> 4 & 0b0011);
		car->types[3] = define_type(car->codage >> 2 & 0b000011);
	}
	else
	{
		car->types[1] = define_type(op->args_type[0]);
		car->types[2] = define_type(op->args_type[1]);
		car->types[3] = define_type(op->args_type[2]);
	}
	car->sizes[1] = define_size(car->types[1], op->label);
	car->sizes[2] = car->op->args > 1 ? define_size(car->types[2], op->label) : 0;
	car->sizes[3] = car->op->args > 2 ? define_size(car->types[3], op->label) : 0;
	car->params[1].integer = get_bytes(car->position + op->codage + 1,
	car->sizes[1], car->types[1], map);
	car->params[2].integer = get_bytes(car->position + op->codage + 1 +
	car->sizes[1], car->sizes[2], car->types[2], map);
	car->params[3].integer = get_bytes(car->position + op->codage + 1 +
	car->sizes[1] + car->sizes[2], car->sizes[3], car->types[3], map);
	return (op->codage + 1 + car->sizes[1] + car->sizes[2] + car->sizes[3]);
}

t_bool	args_ok(t_carriage *car)
{
	if (car->sizes[1] == 0 || (car->op->args_type[0] | car->types[1]) != car->op->args_type[0])
		return (false);
	if (car->op->args > 1 && (car->sizes[2] == 0 ||
		(car->op->args_type[1] | car->types[2]) != car->op->args_type[1]))
		return (false);
	if (car->op->args > 2 && (car->sizes[3] == 0 ||
		(car->op->args_type[2] | car->types[3]) != car->op->args_type[2]))
		return (false);
	return (true);
}

void	introduce(t_champion **champs, t_bool flag_visual) // fix
{
	int i;

	if (!flag_visual)
	{
		i = -1;
		ft_printf("Introducing contestants...\n");
		while (champs[++i])
		{
			ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
				champs[i]->number, champs[i]->header->prog_size,
				champs[i]->header->prog_name, champs[i]->header->comment);
		}
	}
}

void	remove_carriages(t_carriage **list, t_war *war)
{
	t_carriage *del;
	t_carriage *tmp;

	while (*list && (war->cycle - (*list)->last_live >= war->cycles_to_die || war->cycles_to_die <= 0))
	{
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
				(*list)->number, war->cycle - (*list)->last_live, war->cycles_to_die);
		del = *list;
		*list = (*list)->next;
		free(del);
	}
	tmp = *list;
	while (tmp && tmp->next)
	{
		if (war->cycle - tmp->next->last_live >= war->cycles_to_die || war->cycles_to_die <= 0)
		{
			if (war->flag_verbose && war->cycle >= war->flag_dump)
				ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
					tmp->next->number, war->cycle - tmp->next->last_live, war->cycles_to_die);	
			del = tmp->next;
			tmp->next = tmp->next->next;
			free(del);
		}
		else
			tmp = tmp->next;
	}
}

void	checking(t_war *war)
{
	war->cycles_after_check += 1;
	if (war->cycles_after_check >= war->cycles_to_die)
	{
		war->checks += 1;
		remove_carriages(&war->carriages, war);
		if (war->all_lives >= NBR_LIVE || war->checks == MAX_CHECKS)
		{
			war->cycles_to_die -= CYCLE_DELTA;
			war->checks = 0;
			if (war->flag_verbose && war->cycle >= war->flag_dump)
				ft_printf("Cycle to die is now %d\n", war->cycles_to_die);
		}
		war->cycles_after_check = 0;
		war->champs[0]->lives_cur_period = 0;
		war->all_lives = 0;
	}
}

void	run_all_carriages(t_war *war)
{
	t_carriage *car;

	car = war->carriages;
	while (car)
	{
		if (car->op == NULL)
		{
			car->op = get_command(car->number, car->position, war->map, war);
			if (car->op)
				car->cooldown = car->op->cooldown;
			else
				car->position = (car->position + 1) % MEM_SIZE;
		}
		car->cooldown--;
		if (car->op && car->cooldown == 0)
		{
			int delta = get_args(car, war->map, car->op, war);
			if (args_ok(car))
				car->op->func(car, war);
			if (car->carry == false || car->op->code != 0x09)
			{
				adv(war, car->op, delta, car);
				car->position = (car->position + delta) % MEM_SIZE;
			}
			car->op = NULL;
		}
		car = car->next;
	}
}

int		main(int argc, char **argv) 
{
	t_war *war;

	parse_params(argc, argv, war = init());
	parse_champions(war);
	introduce(war->champs, war->flag_visual);
	throw_basic_carriages(war);

	if (war->flag_visual)
		init_curses(war);
	if (war->cycle >= war->flag_dump)
		print_memory(war);
	if (!war->flag_visual && war->cycle == war->flag_dump)
		dump(war); // dump 0

	while (true)
	{
		t_carriage *tmp = war->carriages;
		if (next_cycle(war))
		{
			run_all_carriages(war);
			checking(war);
			if (!war->carriages)
			{
				if (!war->flag_visual && (war->flag_dump == -1 || war->flag_dump >= war->cycle))
				{
					ft_printf("Contestant %d, \"%s\", has won !\n",
						war->last_live->number, war->last_live->header->prog_name);
				}
				break ;
			}
			if (!war->flag_visual && !war->flag_verbose && war->cycle == war->flag_dump)
				dump(war);
		}
	}
	if (war->flag_visual)
		over_curses(war);
	return (0);
}
