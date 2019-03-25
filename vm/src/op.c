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

	// CARRY
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | ld %d r%d\n", car->number, p->params[1].integer, p->params[2].integer);
	}
}


void	op_ldi(t_carriage *car, t_war *war, t_instr_params *p)
{
	int index = car->position + (p->params[1].integer + p->params[2].integer) % IDX_MOD; // only T_DIR
	union converter value;
	value.bytes[0] = war->map[index]->value;
	value.bytes[1] = war->map[index + 1]->value;
	value.bytes[2] = war->map[index + 2]->value;
	value.bytes[3] = war->map[index + 3]->value;


	// ft_printf("%x %x %x %x = %d\n",
	// 	war->map[index]->value,
	// 	war->map[index + 1]->value,
	// 	war->map[index + 2]->value,
	// 	war->map[index + 3]->value,
	// 	value.integer);
	car->reg[p->params[3].integer] = value.integer;
	// ft_printf("LDI Params %ld %#x + %ld %#x = %ld %#x\n",
	// 	p->params[1].integer, p->params[1].integer,
	// 	p->params[2].integer, p->params[2].integer,
	// 	car->reg[p->params[3].integer], car->reg[p->params[3].integer]);
	// ft_printf("LDI Cycle %d, Register %d, Value %ld\n", war->cycle, p->params[3].integer, car->reg[p->params[3].integer]);
	if (war->flag_verbose)
	{
		ft_printf("P    %d | ldi %d %d r%d\n", car->number, p->params[1].integer, p->params[2].integer, p->params[3].integer);
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
			p->params[1].integer,
			p->params[2].integer,
			p->params[1].integer + p->params[2].integer,
			car->position + (p->params[1].integer + p->params[2].integer) % IDX_MOD);
	}
}

void	op_st(t_carriage *car, t_war *war, t_instr_params *p)
{
	
}




void	op_sti(t_carriage *car, t_war *war, t_instr_params *p)
{
	int i;

	// show_args(p);

	unsigned int value_1 = p->params[1].integer;
	unsigned int value_2 = 0;
	unsigned int value_3 = 0;

	if (p->types[2] == T_DIR)
		value_2 = p->params[2].integer;
	else if (p->types[2] == T_REG)
		value_2 = car->reg[p->params[2].integer];

	if (p->types[3] == T_DIR)
		value_3 = p->params[3].integer;
	else if (p->types[3] == T_REG)
		value_2 = car->reg[p->params[3].integer];

	// ft_printf("STI Cycle: %d, VALUE: %ld\n", war->cycle, value_1);

	union converter number;
	number.integer = car->reg[value_1];
	i = -1;
	while (++i < 4)
	{
		war->map[car->position + (value_2 + value_3) % IDX_MOD + i]->value = number.bytes[i];
		war->map[car->position + (value_2 + value_3) % IDX_MOD + i]->cycles_bold = war->cycle;
		war->map[car->position + (value_2 + value_3) % IDX_MOD + i]->color = car->creator->number;
	}
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | sti r%d %d %d\n", car->number, value_1, value_2, value_3);
		ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
			value_2, value_3, value_2 + value_3, car->position + (value_2 + value_3) % IDX_MOD);
	}
}


void	op_fork(t_carriage *car, t_war *war, t_instr_params *p)
{
	t_carriage *new = create_carriage(0, 0, war, car->creator);
	new->position = (car->position + p->params[1].integer) % IDX_MOD;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;
	// new->cooldown = 1;
	// new->player = car->player;
	// copy carry
	// new->op_code = car->op_code;

	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | fork %d (%d)\n", car->number, p->params[1].integer, (car->position + p->params[1].integer) % IDX_MOD);
	}
	
}

void	op_zjmp(t_carriage *car, t_war *war, t_instr_params *p)
{
	if (car->carry == true)
	{
		car->position += (p->params[1].integer % IDX_MOD);
	}

	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | zjmp %d OK\n", car->number, p->params[1].integer % IDX_MOD);
	}
	
}

void	op_new(t_carriage *car, t_war *war, t_instr_params *p)
{


	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P%5d | \n", car->number);
	}
	
}

t_op		op_tab[] =  // [17]
{
//   name       args                                                                     index cycles codage label
	{"live",	1,	{					T_DIR,						0,				0	},	 1,	  10,	0,		0,	 &op_live	},
	{  "ld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	 2,	   5,	1,		0,	   &op_ld	},
	{  "st",	2,	{					T_REG,			T_IND | T_REG,				0	},	 3,	   5,	1,		0,	   &op_st	},
	{"zjmp",	1,	{					T_DIR,						0,				0	},	 9,	  20,	0,		1,	 &op_zjmp	},
	{ "ldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	10,	  25,	1,		1,	  &op_ldi	},
	{ "sti",	3,	{					T_REG,	T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	  25,	1,		1,	  &op_sti	},
	{"fork",	1,	{					T_DIR,						0,				0	},	12,	 800,	0,		1,	 &op_fork	},
	{     0,	0,	{						0,						0,				0	},	 0,	   0,	0,		0,			0	}
};