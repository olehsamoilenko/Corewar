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

// SWAP WITH OP.C FROM SUBJ

void	op_live(int index, t_carriage *car, t_war *war, int *arg)
{
	car->last_live = war->cycle;
	war->map[car->position]->cycles_live = war->cycle;
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | live %d\n", car->player->number, arg[1]);
		ft_printf("Player %d (%s) is said to be alive\n", car->player->number, car->player->header->prog_name);
	}
}

void	op_ld(int index, t_carriage *car, t_war *war, int *arg)
{
	car->reg[arg[2]] = arg[1]; // check < 16
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | ld %d r%d\n", car->player->number, arg[1], arg[2]);
	}
}


void	op_ldi(int index, t_carriage *car, t_war *war, int *arg)
{
	car->reg[arg[3]] = arg[1] + arg[2]; // only T_DIR

	if (war->flag_verbose)
	{
		ft_printf("P    %d | ldi %d %d r%d\n", car->player->number, arg[1], arg[2], arg[3]);
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
			arg[1],
			arg[2],
			arg[1] + arg[2],
			(car->position + arg[1] + arg[2]) % IDX_MOD);
	}
}

void	op_st(int index, t_carriage *car, t_war *war, int *arg)
{
	
}

void	op_sti(int index, t_carriage *car, t_war *war, int *arg)
{
	union magic_header number;
	number.hex = car->reg[arg[1]];
	int i = -1;
	while (++i < 4)
	{
		war->map[car->position + (arg[2] + arg[3]) % IDX_MOD + i]->value = number.bytes[i];
		war->map[car->position + (arg[2] + arg[3]) % IDX_MOD + i]->cycles_bold = war->cycle;
	}
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | sti r%d %d %d\n", car->player->number, arg[1], arg[2], arg[3]);
		ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
			arg[2], arg[3], arg[2] + arg[3], car->position + (arg[2] + arg[3]) % IDX_MOD);
	}
}


void	op_fork(int index, t_carriage *car, t_war *war, int *arg)
{

}



t_op		op_tab[] =  // [17]
{
//   name       args                                                                     index cycles codage label
	{"live",	1,	{					T_DIR,						0,				0	},	 1,	  10,	0,		0, &op_live	},
	{  "ld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	 2,	   5,	1,		0,   &op_ld	},
	{  "st",	2,	{					T_REG,			T_IND | T_REG,				0	},	 3,	   5,	1,		0,   &op_st	},
	{ "ldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	10,	  25,	1,		1,  &op_ldi	},
	{ "sti",	3,	{					T_REG,	T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	  25,	1,		1,  &op_sti	},
	{"fork",	1,	{					T_DIR,						0,				0	},	12,	 800,	0,		1, &op_fork	},
	{     0,	0,	{						0,						0,				0	},	 0,	   0,	0,		0,        0	}
};