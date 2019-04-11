/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   carriage.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	push_carriage(t_carriage *car, t_carriage **list)
{
	car->next = *list;
	*list = car;
}

t_carriage	*create_carriage(t_war *war, t_champion *creator)
{
	t_carriage	*car;
	int			mem_delta;

	mem_delta = MEM_SIZE / chmps_count(war->champs);
	car = ft_memalloc(sizeof(t_carriage));
	car->position = mem_delta * (creator->number - 1);
	car->creator = creator;
	car->number = ++(war->processes_counter);
	war->processes_amount++;
	car->reg[1].integer = -(creator->number);
	car->carry = false;
	return (car);
}
