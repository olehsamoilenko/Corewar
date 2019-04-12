/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_aff(t_carriage *car, t_war *war)
{
	int value_1;

	if (get_value(car, 1, war, 0, &value_1))
	{
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("Aff: %c\n", (char)value_1);
	}
}

t_bool		get_value(t_carriage *car, int num, t_war *war, int index, int *res)
{
	int reg_num;

	if (car->type[num] == T_DIR)
		*res = car->param[num].integer;
	else if (car->type[num] == T_REG)
	{
		if (!correct_reg(reg_num = car->param[num].integer))
			return (false);
		else
			*res = car->reg[reg_num].integer;
	}
	else if (car->type[num] == T_IND)
	{
		t_converter num;
		index %= MEM_SIZE;
		if (index < 0)
			index += MEM_SIZE;
		num.bytes[3] = war->map[(index + 0) % MEM_SIZE]->value;
		num.bytes[2] = war->map[(index + 1) % MEM_SIZE]->value;
		num.bytes[1] = war->map[(index + 2) % MEM_SIZE]->value;
		num.bytes[0] = war->map[(index + 3) % MEM_SIZE]->value;
		*res = num.integer;
	}
	return (true);
}

t_bool		correct_reg(int reg_num)
{
	if (reg_num < 1 || reg_num > 16)
		return (false);
	else
		return (true);
}

t_converter get_from_map(t_war *war, int index)
{
	t_converter reg;

	index %= MEM_SIZE;
	if (index < 0)
		index += MEM_SIZE;
	reg.bytes[3] = war->map[(index + 0) % MEM_SIZE]->value;
	reg.bytes[2] = war->map[(index + 1) % MEM_SIZE]->value;
	reg.bytes[1] = war->map[(index + 2) % MEM_SIZE]->value;
	reg.bytes[0] = war->map[(index + 3) % MEM_SIZE]->value;
	return (reg);
}

void	throw_on_map(t_converter value, t_war *war, t_carriage *car, int index)
{
	int i;

	index %= MEM_SIZE;
	if (index < 0)
		index += MEM_SIZE;
	war->map[(index + 0) % MEM_SIZE]->value = value.bytes[3];
	war->map[(index + 1) % MEM_SIZE]->value = value.bytes[2];
	war->map[(index + 2) % MEM_SIZE]->value = value.bytes[1];
	war->map[(index + 3) % MEM_SIZE]->value = value.bytes[0];
	i = -1;
	while (++i < 4)
	{
		war->map[(index + i) % MEM_SIZE]->cycles_bold = war->cycle;
		war->map[(index + i) % MEM_SIZE]->color = car->creator->number;
	}
}