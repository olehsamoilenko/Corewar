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
		war->cycle += 1;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("It is now cycle %d\n", war->cycle);
		
		// car->cooldown--;
	}
	if (war->flag_visual && (key == KEY_S || war->cycle == war->flag_dump))
	{
		// print_info(war);
		print_memory(war);
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
		res |= map[(start + i) % MEM_SIZE]->value /*& 0xFF*/;
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
	// ft_printf()
	// ft_printf("%d\n", car_pos);
	int index = op_index(map[car_pos]->value); // return op ?
	t_op *op = &op_tab[index];

	if (war->flag_dev)
		ft_printf("\tProcess %d FOUND %02x pos %d, index %d, name %s, cooldown %i\n",
			process, map[car_pos]->value, car_pos, index, op->name, op_tab[index].cooldown);

	if (index == -1)
	{
		if (map[car_pos]->value > 0 && map[car_pos]->value < 17)
			ft_printf("UNKNOWN COMMAND %02x\n", map[car_pos]->value);
		return (NULL);
	}
	return (op);
}

int		define_type_1(int type)
{
	if (type == REG_CODE)
		return(T_REG);
	else if (type == DIR_CODE)
		return(T_DIR);
	else if (type == IND_CODE)
		return (T_IND);
	else
		return (7);
}

int	get_args(t_carriage *car, t_mem_cell *map[], t_op *op, t_war *war)
{
	int v[7];

	int delta = 0;
	delta++;
	int first;
	int second;
	int third;

	if (op->codage == true)
	{
		car->codage = map[car->position + delta]->value;
		first = car->codage >> 6;
		second = car->codage >> 4 & 0b0011;
		third = car->codage >> 2 & 0b000011;
		delta++;
	}
	else
	{
		first = op->args_type[0];
		second = op->args_type[1];
		third = op->args_type[2];
	}

	car->args_ok = true;

	car->types[1] = define_type_1(first);
	// if (car->op->args > 1)
		car->types[2] = define_type_1(second);
	// if (car->op->args > 2)
		car->types[3] = define_type_1(third);

	
	car->sizes[1] = define_size(first, op->label);
	
	if (car->op->args > 1)
		car->sizes[2] = define_size(second, op->label);
	else
		car->sizes[2] = 0;

	if (car->op->args > 2)
		car->sizes[3] = define_size(third, op->label);
	else
		car->sizes[3] = 0;

	unsigned int arg_1 = 0, arg_2 = 0, arg_3 = 0;
	arg_1 = get_bytes(car->position + delta, car->sizes[1], car->types[1], map);
	delta += car->sizes[1];
	arg_2 = get_bytes(car->position + delta, car->sizes[2], car->types[2], map);
	delta += car->sizes[2];
	arg_3 = get_bytes(car->position + delta, car->sizes[3], car->types[3], map);
	delta += car->sizes[3];

	car->params[1].integer = arg_1;
	car->params[2].integer = arg_2;
	car->params[3].integer = arg_3;

	if ((car->op->args_type[0] | car->types[1]) != car->op->args_type[0])
		car->args_ok = false;
	if (car->op->args > 1 && (car->op->args_type[1] | car->types[2]) != car->op->args_type[1])
		car->args_ok = false;
	if (car->op->args > 2 && (car->op->args_type[2] | car->types[3]) != car->op->args_type[2])
		car->args_ok = false;

	return (delta);
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

void	verbose_death(t_carriage *car, t_war *war)
{
	ft_printf("Process %d hasn't lived for %d cycles (CTD %d)\n",
		car->number, war->cycle - car->last_live, war->cycles_to_die);
}

void	remove_carriages(t_carriage **list, t_war *war)
{
	// show_carriages(war);

	t_carriage *del;

	while (*list && (war->cycle - (*list)->last_live >= war->cycles_to_die || war->cycles_to_die <= 0))
	{
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			verbose_death(*list, war);
		del = *list;
		*list = (*list)->next;
		free(del);
	}

	t_carriage *tmp = *list;
	while (tmp && tmp->next)
	{
		// ft_printf("Checked %d\n", tmp->next->number);
		if (war->cycle - tmp->next->last_live >= war->cycles_to_die || war->cycles_to_die <= 0)
		{
			if (war->flag_verbose && war->cycle >= war->flag_dump)
				verbose_death(tmp->next, war);
			del = tmp->next;
			tmp->next = tmp->next->next;
			free(del);
		}
		else
			tmp = tmp->next;
	}

	// show_carriages(war);
}

void	checking(t_war *war)
{
	war->cycles_after_check += 1;
	// ft_printf("LIVES: %d\n", war->champs[0]->lives_cur_period);
	if (war->cycles_after_check >= war->cycles_to_die)
	{
		war->checks += 1;
		remove_carriages(&war->carriages, war);
		// ft_printf("LIVES %d\n", war->all_lives);
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



// 42				TOO BIG
// CAR				LONG
// GAGNANT			OK
// OCTOBRE			LONG
// BARRIERE			TOO BIG
// BEE_GEES			LONG
// BIGZORK			OK
// EX				OK
// FLUTTERSHY		OK
// HELLTRAIN		... 26469 wrong verbose
// JUMPER			OK
// MAXIDEF			OK
// MORTEL			OK
// SLIDER			OK
// TOTO				OK
// TURTLE			OK
// ZORK				OK



int		main(int argc, char **argv) 
{
	t_war *war = init();
	parse_params(argc, argv, war);
	int mem_delta = MEM_SIZE / champions_count(war->champs);
	parse_champions(war->champs, war->map, mem_delta);
	war->last_live = war->champs[0];
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

	int i = -1;
	while (true)
	{
		t_carriage *tmp = war->carriages;

		

		next_cycle(war);
		
		while (tmp)
		{
			t_carriage *car = tmp;
			// ft_printf("%d\n", tmp->number);
			if (car->op == NULL)
			{
				// ft_printf("%d\n", tmp->number);
				
				car->op = get_command(car->number, car->position, war->map, war);
				if (car->op)
				{
					car->cooldown = car->op->cooldown;
					// car->codage = war->map[car->position + 1]->value;
				}
				else
					car->position = (car->position + 1) % MEM_SIZE;
			}
			car->cooldown--;
			if (car->op && car->cooldown == 0)
			{
				
				int delta = get_args(car, war->map, car->op, war);

				if (car->args_ok)
					car->op->func(car, war);
				if (car->op->code == 0x09 && car->carry == true) // zjmp
				{

				}
				else
				{
					adv(war, car->op, delta, car);
					car->position = (car->position + delta) % MEM_SIZE;
				}
				car->op = NULL;
			}
			if (war->flag_visual)
				print_memory(war);
			tmp = tmp->next;
			
		}
		
		checking(war);

		if (!war->carriages)
		{
			if (war->flag_dump == -1 || war->flag_dump >= war->cycle)
			{
				ft_printf("Contestant %d, \"%s\", has won !\n",
					war->last_live->number, war->last_live->header->prog_name);

			}
			break ;
		}
		if (!war->flag_visual && !war->flag_verbose && war->cycle == war->flag_dump)
		{
			dump(war);
			// ft_printf("sd");
			// break ;
		}
	}

	// show_carriages(war);
	
	if (war->flag_visual)
		over_curses(war);
	// show_carriages(war);
	// if (war->flag_dump == -1)
	// 	system("leaks vm");
	return (0);
}
