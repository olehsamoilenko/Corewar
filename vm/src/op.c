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

void	op_live(t_carriage *car, t_war *war, t_instr_params *p)
{
	car->last_live = war->cycle;
	war->map[car->position]->cycles_live = war->cycle;
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | live %d\n", car->number, p->params[1].integer);
		t_champion *player = find_champ(p->params[1].integer, war);
		ft_printf("Player %d (%s) is said to be alive\n",
			player->number, player->header->prog_name);
		

		// ft_printf("ADV %d (%x -> %x)\n", delta, car->position, car->position + delta);
	}
}

void	op_ld(t_carriage *car, t_war *war, t_instr_params *p)
{
	car->reg[p->params[2].integer] = p->params[1].integer; // check < 16
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | ld %d r%d\n", car->number, p->params[1].integer, p->params[2].integer);
	}
}


void	op_ldi(t_carriage *car, t_war *war, t_instr_params *p)
{
	car->reg[p->params[3].integer] = p->params[1].integer + p->params[2].integer; // only T_DIR

	if (war->flag_verbose)
	{
		ft_printf("P    %d | ldi %d %d r%d\n", car->number, p->params[1].integer, p->params[2].integer, p->params[3].integer);
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
			p->params[1].integer,
			p->params[2].integer,
			p->params[1].integer + p->params[2].integer,
			(car->position + p->params[1].integer + p->params[2].integer) % IDX_MOD);
	}
}

void	op_st(t_carriage *car, t_war *war, t_instr_params *p)
{
	
}

void	op_sti(t_carriage *car, t_war *war, t_instr_params *p)
{
	union converter number;
	number.integer = car->reg[p->params[1].integer];
	int i = -1;
	while (++i < 4)
	{
		war->map[car->position + (p->params[2].integer + p->params[3].integer) % IDX_MOD + i]->value = number.bytes[i];
		war->map[car->position + (p->params[2].integer + p->params[3].integer) % IDX_MOD + i]->cycles_bold = war->cycle;
	}
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | sti r%d %d %d\n", car->number, p->params[1].integer, p->params[2].integer, p->params[3].integer);
		ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
			p->params[2].integer, p->params[3].integer, p->params[2].integer + p->params[3].integer, car->position + (p->params[2].integer + p->params[3].integer) % IDX_MOD);
	}
}


void	op_fork(t_carriage *car, t_war *war, t_instr_params *p)
{
	t_carriage *new = create_carriage(0, 0, war);
	new->position = (car->position + p->params[1].integer) % IDX_MOD;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	// new->player = car->player;
	// copy carry
	// new->op_code = car->op_code;

	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | fork %d (%d)\n", car->number, p->params[1].integer, (car->position + p->params[1].integer) % IDX_MOD);
	}
	
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