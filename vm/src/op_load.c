/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_load.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_live(t_carriage *car, t_war *war)
{
	t_champion *player;

	car->last_live = war->cycle;
	war->map[car->position]->cycles_live = war->cycle;
	player = find_champ(car->param[1].integer, war);
	if (player != NULL)
	{
		player->last_live = war->cycle;
		(player->lives_cur_period)++;
		war->last_live = player;
	}
	war->all_lives += 1;
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P %4d | live %d\n", car->number, car->param[1].integer);
		if (player != NULL)
		{
			ft_printf("Player %d (%s) is said to be alive\n",
				player->number, player->header->prog_name);
		}
	}
}

void	op_ld(t_carriage *car, t_war *war)
{
	int reg_num;

	if (correct_reg(reg_num = car->param[2].integer))
	{
		get_value(car, 1, war,
			car->position + car->param[1].integer % IDX_MOD);
		car->reg[reg_num].integer = car->value[1];
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | ld %d r%d\n", car->number,
			car->reg[reg_num].integer, car->param[2].integer);
	}
}

void	op_ldi(t_carriage *car, t_war *war)
{
	int reg_num;
	int index;

	if (get_value(car, 1, war, car->position + car->param[1].integer) &&
	get_value(car, 2, war, 0) &&
						correct_reg(reg_num = car->param[3].integer))
	{
		index = car->position + (car->value[1] + car->value[2]) % IDX_MOD;
		car->reg[reg_num] = get_from_map(war, index);
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | ldi %d %d r%d\n", car->number, car->value[1],
													car->value[2], reg_num);
			ft_printf("       | -> load from %d + %d = %d ",
				car->value[1], car->value[2], car->value[1] + car->value[2]);
			ft_printf("(with pc and mod %d)\n", index);
		}
	}
}

void	op_lld(t_carriage *car, t_war *war)
{
	int reg_num;

	if (correct_reg(reg_num = car->param[2].integer) &&
	get_value(car, 1, war, car->position + car->param[1].integer))
	{
		car->reg[reg_num].integer = car->value[1];
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
			ft_printf("P %4d | lld %d r%d\n", car->number,
			car->reg[reg_num].integer, reg_num);
	}
}

void	op_lldi(t_carriage *car, t_war *war)
{
	int reg_num;
	int index;

	if (get_value(car, 1, war, car->position + car->param[1].integer) &&
	get_value(car, 2, war, 0) &&
						correct_reg(reg_num = car->param[3].integer))
	{
		index = car->position + car->value[1] + car->value[2];
		car->reg[reg_num] = get_from_map(war, index);
		car->carry = car->reg[reg_num].integer == 0 ? true : false;
		if (war->flag_verbose && war->cycle >= war->flag_dump)
		{
			ft_printf("P %4d | lldi %d %d r%d\n", car->number, car->value[1],
													car->value[2], reg_num);
			ft_printf("       | -> load from %d + %d = %d (with pc %d)\n",
				car->value[1], car->value[2], car->value[1] + car->value[2],
																	index);
		}
	}
}
