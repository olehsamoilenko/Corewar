/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_general.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_st(t_carriage *car, t_war *war)
{
	int r1;
	int r2;

	if (correct_reg(r1 = car->param[1].integer))
	{
		if (car->type[2] == T_IND)
			throw_on_map(car->reg[r1], war, car,
			car->position + car->param[2].integer % IDX_MOD);
		else if (car->type[2] == T_REG)
		{
			if (!correct_reg(r2 = car->param[2].integer))
				return ;
			else
			{
				car->reg[r2].bytes[0] = car->reg[r1].bytes[0];
				car->reg[r2].bytes[1] = car->reg[r1].bytes[1];
				car->reg[r2].bytes[2] = car->reg[r1].bytes[2];
				car->reg[r2].bytes[3] = car->reg[r1].bytes[3];
			}
		}
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | st r%d %d\n",
			car->number, r1, car->param[2].integer);
	}
}

void	op_sti(t_carriage *car, t_war *war)
{
	int r1;
	int value_2;
	int value_3;
	int index;

	if (get_value(car, 2, war, car->position +
	car->param[2].integer % IDX_MOD, &value_2) &&
	get_value(car, 3, war, car->position +
	car->param[2].integer % IDX_MOD, &value_3) &&
	correct_reg(r1 = car->param[1].integer))
	{
		index = car->position + (value_2 + value_3) % IDX_MOD;
		throw_on_map(car->reg[r1], war, car, index);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | sti r%d %d %d\n", car->number, r1, value_2,
																	value_3);
			ft_printf("       | -> store to %d + %d = %d ",
				value_2, value_3, value_2 + value_3);
			ft_printf("(with pc and mod %d)\n", index);
		}
	}
}

void	op_fork(t_carriage *car, t_war *war)
{
	int			i;
	t_carriage	*new;

	new = create_carriage(war, car->creator);
	new->position = car->position + car->param[1].integer % IDX_MOD;
	while (new->position < 0)
		new->position += MEM_SIZE;
	new->position %= MEM_SIZE;
	push_carriage(new, &war->carriages);
	i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;
	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("P %4d | fork %d (%d)\n", car->number,
		car->param[1].integer, car->position +
									car->param[1].integer % IDX_MOD);
}

void	op_lfork(t_carriage *car, t_war *war)
{
	t_carriage	*new;
	int			i;

	new = create_carriage(war, car->creator);
	new->position = car->position + car->param[1].integer;
	while (new->position < 0)
		new->position += MEM_SIZE;
	new->position %= MEM_SIZE;
	push_carriage(new, &war->carriages);
	i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P %4d | lfork %d (%d)\n", car->number,
			car->param[1].integer, car->position + car->param[1].integer);
	}
}

void	op_zjmp(t_carriage *car, t_war *war)
{
	char *jump_status;

	if (car->carry == true)
	{
		car->position = (car->position + (car->param[1].integer % IDX_MOD))
																% MEM_SIZE;
		if (car->position < 0)
			car->position += MEM_SIZE;
		jump_status = "OK";
	}
	else
		jump_status = "FAILED";
	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("P %4d | zjmp %d %s\n", car->number,
		car->param[1].integer, jump_status);
}
