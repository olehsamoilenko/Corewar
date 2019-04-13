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

void	get_command(t_war *war, t_carriage *car)
{
	int i;

	if (car->op == NULL)
	{
		i = -1;
		while (g_op_tab[++i].name)
		{
			if (g_op_tab[i].code == war->map[car->position]->value)
				car->op = &g_op_tab[i];
		}
		if (car->op)
			car->cooldown = car->op->cooldown;
		else
			car->position = (car->position + 1) % MEM_SIZE;
	}
}

t_bool	args_ok(t_carriage *car)
{
	if (car->size[1] == 0 ||
		(car->op->args_type[0] | car->type[1]) != car->op->args_type[0])
		return (false);
	if (car->op->args > 1 && (car->size[2] == 0 ||
		(car->op->args_type[1] | car->type[2]) != car->op->args_type[1]))
		return (false);
	if (car->op->args > 2 && (car->size[3] == 0 ||
		(car->op->args_type[2] | car->type[3]) != car->op->args_type[2]))
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
		get_command(war, car);
		if (car->op && --car->cooldown == 0)
		{
			delta = get_args(car, war);
			if (args_ok(car))
				car->op->func(car, war);
			if (car->carry == false || car->op->code != 0x09)
			{
				adv(war, delta, car);
				car->position = (car->position + delta) % MEM_SIZE;
			}
			car->op = NULL;
		}
		car = car->next;
	}
}
