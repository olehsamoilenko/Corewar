/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_carriages.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_op	*get_command(int pos, t_map_cell **map)
{
	int i;

	i = -1;
	while (g_op_tab[++i].name)
	{
		if (g_op_tab[i].code == map[pos]->value)
			return (&g_op_tab[i]);
	}
	return (NULL);
}

t_bool	args_ok(t_carriage *car)
{
	if (car->sizes[1] == 0 ||
		(car->op->args_type[0] | car->types[1]) != car->op->args_type[0])
		return (false);
	if (car->op->args > 1 && (car->sizes[2] == 0 ||
		(car->op->args_type[1] | car->types[2]) != car->op->args_type[1]))
		return (false);
	if (car->op->args > 2 && (car->sizes[3] == 0 ||
		(car->op->args_type[2] | car->types[3]) != car->op->args_type[2]))
		return (false);
	return (true);
}

void	run_carriages(t_war *war)
{
	t_carriage	*car;
	int			delta;

	car = war->carriages;
	while (car)
	{
		if (car->op == NULL)
		{
			if ((car->op = get_command(car->position, war->map)))
				car->cooldown = car->op->cooldown;
			else
				car->position = (car->position + 1) % MEM_SIZE;
		}
		if (car->op && --car->cooldown == 0)
		{
			delta = get_args(car, war);
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
