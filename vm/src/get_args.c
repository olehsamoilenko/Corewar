/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

int		define_type(int type)
{
	if (type == REG_CODE)
		return (T_REG);
	else if (type == DIR_CODE)
		return (T_DIR);
	else if (type == IND_CODE)
		return (T_IND);
	else
		return (0b111);
}

int		get_args(t_carriage *car, t_war *war)
{
	// t_car->op *op;

	// op = car->op;
	if (car->op->codage == true)
	{
		car->codage = war->map[(car->position + 1) % MEM_SIZE]->value;
		car->types[1] = define_type(car->codage >> 6);
		car->types[2] = define_type(car->codage >> 4 & 0b0011);
		car->types[3] = define_type(car->codage >> 2 & 0b000011);
	}
	else
	{
		car->types[1] = define_type(car->op->args_type[0]);
		car->types[2] = define_type(car->op->args_type[1]);
		car->types[3] = define_type(car->op->args_type[2]);
	}
	car->sizes[1] = define_size(car->types[1], car->op->label);
	car->sizes[2] = car->op->args > 1 ? define_size(car->types[2], car->op->label) : 0;
	car->sizes[3] = car->op->args > 2 ? define_size(car->types[3], car->op->label) : 0;
	car->params[1].integer = get_bytes(car->position + car->op->codage + 1,
	car->sizes[1], car->types[1], war->map);
	car->params[2].integer = get_bytes(car->position + car->op->codage + 1 +
	car->sizes[1], car->sizes[2], car->types[2], war->map);
	car->params[3].integer = get_bytes(car->position + car->op->codage + 1 +
	car->sizes[1] + car->sizes[2], car->sizes[3], car->types[3], war->map);
	return (car->op->codage + 1 + car->sizes[1] + car->sizes[2] + car->sizes[3]);
}
