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
	// show_args(p, war);

	car->last_live = war->cycle;
	war->map[car->position]->cycles_live = war->cycle;
	t_champion *player = find_champ(p->params[1].integer, war);
	if (player != NULL)
	{
		player->last_live = war->cycle;
		(player->lives_cur_period)++;

	}
	
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | live %d\n", car->number, p->params[1].integer);
		
		if (player != NULL)
		{
			ft_printf("Player %d (%s) is said to be alive\n",
				player->number, player->header->prog_name);

		}
		

		// ft_printf("ADV %d (%x -> %x)\n", delta, car->position, car->position + delta);
	}
}

void	op_ld(t_carriage *car, t_war *war, t_instr_params *p)
{
	// show_args(p, war);
	int reg_num = p->params[2].integer; // check < 16

	// direct to register must be inversed
	// here params don't need UNION

	car->reg[reg_num].integer = p->params[1].integer;
	// car->reg[reg_num].bytes[1] = p->params[1].bytes[2];
	// car->reg[reg_num].bytes[2] = p->params[1].bytes[1];
	// car->reg[reg_num].bytes[3] = p->params[1].bytes[0];

	// ft_printf("%d\n", car->reg[reg_num].integer);


	// ft_printf("%02x ", p->params[1].bytes[0]);
	// ft_printf("%02x ", p->params[1].bytes[1]);
	// ft_printf("%02x ", p->params[1].bytes[2]);
	// ft_printf("%02x ", p->params[1].bytes[3]);
	// ft_printf("\n");

	// ft_printf("%02x ", car->reg[reg_num].bytes[0]);
	// ft_printf("%02x ", car->reg[reg_num].bytes[1]);
	// ft_printf("%02x ", car->reg[reg_num].bytes[2]);
	// ft_printf("%02x ", car->reg[reg_num].bytes[3]);
	// ft_printf("\n");

	// CARRY
	
	// verbose
	if (war->flag_verbose)
	{
		ft_printf("P    %d | ld %d r%d\n", car->number, p->params[1].integer, p->params[2].integer);
	}
}


void	op_ldi(t_carriage *car, t_war *war, t_instr_params *p)
{
	// show_args(p, war);

	int index = car->position + (p->params[1].integer + p->params[2].integer) % IDX_MOD; // only T_DIR
	

	int reg_number = p->params[3].integer;
	// ft_printf("%d\n", reg_number);

	car->reg[reg_number].bytes[3] = war->map[index + 0]->value;
	car->reg[reg_number].bytes[2] = war->map[index + 1]->value;
	car->reg[reg_number].bytes[1] = war->map[index + 2]->value;
	car->reg[reg_number].bytes[0] = war->map[index + 3]->value;


	// ft_printf("%02x ", car->reg[reg_number].bytes[0]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[1]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[2]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[3]);
	// ft_printf("\n");



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


void	show_union(union converter a)
{
	ft_printf("%ld : ", a.integer);
	int i = -1;
	while (++i < 4)
	{
		ft_printf("%02x ", a.bytes[i]);
	}
	ft_printf("\n");
}


void	op_sti(t_carriage *car, t_war *war, t_instr_params *p)
{
	int i;

	// show_args(p, war);

	unsigned int value_1 = p->params[1].integer;
	unsigned int value_2 = 0;
	unsigned int value_3 = 0;

	// show_union(p->params[3]); // PROBLEM

	if (p->types[2] == T_DIR)
		value_2 = p->params[2].integer;
	else if (p->types[2] == T_REG)
		value_2 = car->reg[p->params[2].integer].integer;

	

	if (p->types[3] == T_DIR)
		value_3 = p->params[3].integer;
	else if (p->types[3] == T_REG)
		value_3 = car->reg[p->params[3].integer].integer;

	// ft_printf("STI Cycle: %d, VALUE: %ld\n", war->cycle, value_1);

	union converter number;
	number.integer = car->reg[value_1].integer;

	// ft_printf("%02x ", number.bytes[0]);
	// ft_printf("%02x ", number.bytes[1]);
	// ft_printf("%02x ", number.bytes[2]);
	// ft_printf("%02x ", number.bytes[3]);
	// ft_printf("\n");

	war->map[car->position + (value_2 + value_3) % IDX_MOD + 3]->value = number.bytes[0];
	war->map[car->position + (value_2 + value_3) % IDX_MOD + 2]->value = number.bytes[1];
	war->map[car->position + (value_2 + value_3) % IDX_MOD + 1]->value = number.bytes[2];
	war->map[car->position + (value_2 + value_3) % IDX_MOD + 0]->value = number.bytes[3];

	i = -1;
	while (++i < 4)
	{
		
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
//   name       args                                                                      code cycles codage label
	{"live",	1,	{					T_DIR,						0,				0	},	 1,	  10,	0,		0,	 &op_live	},
	{  "ld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	 2,	   5,	1,		0,	   &op_ld	},
	{  "st",	2,	{					T_REG,			T_IND | T_REG,				0	},	 3,	   5,	1,		0,	   &op_st	},
	{"zjmp",	1,	{					T_DIR,						0,				0	},	 9,	  20,	0,		1,	 &op_zjmp	},
	{ "ldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	10,	  25,	1,		1,	  &op_ldi	},
	{ "sti",	3,	{					T_REG,	T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	  25,	1,		1,	  &op_sti	},
	{"fork",	1,	{					T_DIR,						0,				0	},	12,	 800,	0,		1,	 &op_fork	},
	{     0,	0,	{						0,						0,				0	},	 0,	   0,	0,		0,			0	}
};