/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
   
t_bool		get_value(t_carriage *car, int num, t_war *war, int index, int *res)
{
	if (car->types[num] == T_DIR)
		*res = car->params[num].integer;
	else if (car->types[num] == T_REG)
	{
		int reg_num = car->params[num].integer;
		if (reg_num < 1 || reg_num > 16)
			return (false);
		*res = car->reg[reg_num].integer;
	}
	else if (car->types[num] == T_IND)
	{
		union converter num;
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

t_bool		check_carry(int value)
{
	if (value == 0)
		return (true);
	else
		return (false);
}

union converter get_from_map(t_war *war, int index)
{
	index %= MEM_SIZE;
	if (index < 0)
		index += MEM_SIZE;
	union converter reg;
	reg.bytes[3] = war->map[(index + 0) % MEM_SIZE]->value;
	reg.bytes[2] = war->map[(index + 1) % MEM_SIZE]->value;
	reg.bytes[1] = war->map[(index + 2) % MEM_SIZE]->value;
	reg.bytes[0] = war->map[(index + 3) % MEM_SIZE]->value;
	return (reg);
}

void	throw_on_map(union converter value, t_war *war, t_carriage *car, int index)
{
	index %= MEM_SIZE;
	if (index < 0)
		index += MEM_SIZE;
	war->map[(index + 0) % MEM_SIZE]->value = value.bytes[3];
	war->map[(index + 1) % MEM_SIZE]->value = value.bytes[2];
	war->map[(index + 2) % MEM_SIZE]->value = value.bytes[1];
	war->map[(index + 3) % MEM_SIZE]->value = value.bytes[0];
	int i = -1;
	while (++i < 4)
	{
		war->map[(index + i) % MEM_SIZE]->cycles_bold = war->cycle;
		war->map[(index + i) % MEM_SIZE]->color = car->creator->number;
	}

}

void	op_ld(t_carriage *car, t_war *war)
{
	int reg_num;

	if (correct_reg(reg_num = car->params[2].integer))
	{
		get_value(car, 1, war,
			car->position + car->params[1].integer % IDX_MOD,
			&car->reg[reg_num].integer);
		car->carry = check_carry(car->reg[reg_num].integer);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | ld %d r%d\n", car->number,
			car->reg[reg_num].integer, car->params[2].integer);
	}
}

void	op_ldi(t_carriage *car, t_war *war)
{
	int value_1;
	int value_2;
	int reg_num;

	if (get_value(car, 1, war, car->position + car->params[1].integer, &value_1) &&
	get_value(car, 2, war, 0, &value_2) && correct_reg(reg_num = car->params[3].integer))
	{
		int index = car->position + (value_1 + value_2) % IDX_MOD;
		car->reg[reg_num] = get_from_map(war, index);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | ldi %d %d r%d\n", car->number, value_1, value_2, reg_num);
			ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
				value_1, value_2, value_1 + value_2, index);
		}
	}
}

void	op_lld(t_carriage *car, t_war *war)
{
	int reg_num;
	if (correct_reg(reg_num = car->params[2].integer) &&
	get_value(car, 1, war, car->position + car->params[1].integer,
	&car->reg[reg_num].integer))
	{
		car->carry = check_carry(car->reg[reg_num].integer);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | lld %d r%d\n", car->number,
			car->reg[reg_num].integer, reg_num);
	}
}

void	op_lldi(t_carriage *car, t_war *war)
{
	int value_1;
	int value_2;
	int reg_num;
	if (get_value(car, 1, war, car->position + car->params[1].integer, &value_1) &&
	get_value(car, 2, war, 0, &value_2) && correct_reg (reg_num = car->params[3].integer))
	{
		int index = car->position + value_1 + value_2; // only T_DIR
		car->reg[reg_num] = get_from_map(war, index);
		car->carry = check_carry(car->reg[reg_num].integer);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | lldi %d %d r%d\n", car->number, value_1, value_2, reg_num);
			ft_printf("       | -> load from %d + %d = %d (with pc %d)\n",
				value_1, value_2, value_1 + value_2, index);
		}
	}
}

void	op_live(t_carriage *car, t_war *war)
{
	// show_args(p, war);

	car->last_live = war->cycle;
	war->map[car->position]->cycles_live = war->cycle;
	t_champion *player = find_champ(car->params[1].integer, war);
	if (player != NULL)
	{
		player->last_live = war->cycle;
		(player->lives_cur_period)++;
		war->last_live = player;
	}
	war->all_lives += 1;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P %4d | live %d\n", car->number, car->params[1].integer);
		if (player != NULL)
		{
			ft_printf("Player %d (%s) is said to be alive\n",
				player->number, player->header->prog_name);
		}
	}
}

void	op_st(t_carriage *car, t_war *war)
{
	int r1;
	int r2;

	if (correct_reg(r1 = car->params[1].integer))
	{
		if (car->types[2] == T_IND)
			throw_on_map(car->reg[r1], war, car,
			car->position + car->params[2].integer % IDX_MOD);
		else if (car->types[2] == T_REG)
		{
			if (!correct_reg(r2 = car->params[2].integer))
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
			car->number, r1, car->params[2].integer);
	}
}

void	op_sti(t_carriage *car, t_war *war)
{
	int r1;
	int value_2;
	int value_3;
	int index;

	if (get_value(car, 2, war, car->position +
	car->params[2].integer % IDX_MOD, &value_2) &&
	get_value(car, 3, war, car->position +
	car->params[2].integer % IDX_MOD, &value_3) &&
	correct_reg(r1 = car->params[1].integer))
	{
		index = car->position + (value_2 + value_3) % IDX_MOD;
		throw_on_map(car->reg[r1], war, car, index);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | sti r%d %d %d\n", car->number, r1, value_2, value_3);
			ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
				value_2, value_3, value_2 + value_3, index);
		}
	}
}


void	op_fork(t_carriage *car, t_war *war)
{
	t_carriage *new = create_carriage(0, 0, war, car->creator);
	new->position = car->position + car->params[1].integer % IDX_MOD;
	while (new->position < 0)
		new->position += MEM_SIZE;
	new->position %= MEM_SIZE;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;

	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("P %4d | fork %d (%d)\n", car->number,
		car->params[1].integer, car->position + car->params[1].integer % IDX_MOD);
}

void	op_lfork(t_carriage *car, t_war *war)
{
	t_carriage *new = create_carriage(0, 0, war, car->creator);
	new->position = car->position + car->params[1].integer;
	while (new->position < 0)
		new->position += MEM_SIZE;
	new->position %= MEM_SIZE;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;

	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P %4d | lfork %d (%d)\n", car->number, car->params[1].integer,
			car->position + car->params[1].integer);
	}
}

void	op_zjmp(t_carriage *car, t_war *war)
{
	char *jump_status;
	
	if (car->carry == true)
	{
		car->position = (car->position + (car->params[1].integer % IDX_MOD)) % MEM_SIZE;
		if (car->position < 0)
			car->position += MEM_SIZE;
		jump_status = "OK";
	}
	else
		jump_status = "FAILED";

	if (war->flag_verbose && war->cycle >= war->flag_dump)
		ft_printf("P %4d | zjmp %d %s\n", car->number,
		car->params[1].integer, jump_status);
}

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
		car->carry = check_carry(car->reg[r3].integer);
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
		car->carry = check_carry(car->reg[r3].integer);
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
		car->carry = check_carry(car->reg[reg_num].integer);
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
		car->carry = check_carry(car->reg[reg_num].integer);
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
		car->carry = check_carry(car->reg[reg_num].integer);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | xor %d %d r%d\n",
			car->number, value_1, value_2, reg_num);
	}
}

void	op_aff(t_carriage *car, t_war *war)
{
	int value_1;
	if (get_value(car, 1, war, 0, &value_1))
	{
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("Aff: %c\n", (char)value_1);
	}
}

