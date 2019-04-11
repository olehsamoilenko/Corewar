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

t_carriage	*create_carriage(int position, int player, t_war *war, t_champion *creator)
{
	t_carriage *car;

	car = ft_memalloc(sizeof(t_carriage));
	car->position = position;
	car->creator = creator;
	car->number = ++(war->processes_counter);
	war->processes_amount++;
	car->reg[1].integer = -player;
	car->carry = false;
	car->args_ok = true;
	return (car);
}
