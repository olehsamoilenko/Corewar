/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_logic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_add(t_carriage *car, t_war *war)
{
	int r1;
	int r2;
	int r3;
	
	if (correct_reg(r1 = car->params[1].integer) &&
	correct_reg(r2 = car->params[2].integer) &&
	correct_reg(r3 = car->params[3].integer))
	{
		car->reg[r3].integer = car->reg[r1].integer + car->reg[r2].integer;
		car->carry = car->reg[r3].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | add r%d r%d r%d\n", car->number, r1, r2, r3);
	}
}

void	op_sub(t_carriage *car, t_war *war)
{
	int r1;
	int r2;
	int r3;
	
	if (correct_reg(r1 = car->params[1].integer) &&
	correct_reg(r2 = car->params[2].integer) &&
	correct_reg(r3 = car->params[3].integer))
	{
		car->reg[r3].integer = car->reg[r1].integer - car->reg[r2].integer;
		car->carry = car->reg[r3].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | sub r%d r%d r%d\n", car->number, r1, r2, r3);
	}
}

void	op_and(t_carriage *car, t_war *war)
{
	int value_1;
	int value_2;
	int reg_num;

	if (get_value(car, 1, war,
	car->position + car->params[1].integer % IDX_MOD, &value_1) &&
	get_value(car, 2, war,
	car->position + car->params[2].integer % IDX_MOD, &value_2) &&
	correct_reg(reg_num = car->params[3].integer))
	{
		car->reg[reg_num].integer = value_1 & value_2;
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | and %d %d r%d\n",
			car->number, value_1, value_2, reg_num);
	}
}

void	op_or(t_carriage *car, t_war *war)
{
	int value_1;
	int value_2;
	int reg_num;

	if (get_value(car, 1, war,
	car->position + car->params[1].integer % IDX_MOD, &value_1) &&
	get_value(car, 2, war,
	car->position + car->params[2].integer % IDX_MOD, &value_2) &&
	correct_reg(reg_num = car->params[3].integer))
	{
		car->reg[reg_num].integer = value_1 | value_2;
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | or %d %d r%d\n",
			car->number, value_1, value_2, reg_num);
	}
}

void	op_xor(t_carriage *car, t_war *war)
{
	int value_1;
	int value_2;
	int reg_num;

	if (get_value(car, 1, war,
	car->position + car->params[1].integer % IDX_MOD, &value_1) &&
	get_value(car, 2, war,
	car->position + car->params[2].integer % IDX_MOD, &value_2) &&
	correct_reg(reg_num = car->params[3].integer))
	{
		car->reg[reg_num].integer = value_1 ^ value_2;
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | xor %d %d r%d\n",
			car->number, value_1, value_2, reg_num);
	}
}