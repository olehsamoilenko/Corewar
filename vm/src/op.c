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
		ft_printf("P%5d | live %d\n", car->number, car->params[1].integer);
		
		if (player != NULL)
		{
			ft_printf("Player %d (%s) is said to be alive\n",
				player->number, player->header->prog_name);

		}
		

		// ft_printf("ADV %d (%x -> %x)\n", delta, car->position, car->position + delta);
	}
}

void	op_ld(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	t_bool sucess = true;

	int reg_num = car->params[2].integer;

	if (reg_num < 1 || reg_num > 16)
		return ;

	car->reg[reg_num].integer = car->params[1].integer;

	if (car->reg[reg_num].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump && sucess)
	{
		ft_printf("P%5d | ld %d r%d\n", car->number, car->params[1].integer, car->params[2].integer);
	}

}

int		get_value(int param, int type, union converter *reg)
{
	if (type == T_DIR)
		return (param);
	else if (type == T_REG)
		return (reg[param].integer);
	return (0);
}


void	op_ldi(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1 = get_value(car->params[1].integer, car->types[1], car->reg);
	int value_2 = get_value(car->params[2].integer, car->types[2], car->reg);

	
	
	// if (car->types[2] == T_DIR)
	// 	value_2 = car->params[2].integer;
	// else if (car->types[2] == T_REG)
	// 	value_2 = car->reg[car->params[2].integer].integer;

	int index = car->position + (value_1 + value_2) % IDX_MOD; // only T_DIR
	

	car->reg[car->params[3].integer].bytes[3] = war->map[index + 0]->value;
	car->reg[car->params[3].integer].bytes[2] = war->map[index + 1]->value;
	car->reg[car->params[3].integer].bytes[1] = war->map[index + 2]->value;
	car->reg[car->params[3].integer].bytes[0] = war->map[index + 3]->value;


	// ft_printf("%02x ", car->reg[reg_number].bytes[0]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[1]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[2]);
	// ft_printf("%02x ", car->reg[reg_number].bytes[3]);
	// ft_printf("\n");



	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | ldi %d %d r%d\n", car->number, value_1, value_2, car->params[3].integer);
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
			value_1,
			value_2,
			value_1 + value_2,
			car->position + (value_1 + value_2) % IDX_MOD);
	}


}

void	op_lldi(t_carriage *car, t_war *war)
{
	// show_args(war, car);
	// ft_printf("%d\n", car->codage);

	int value_1 = get_value(car->params[1].integer, car->types[1], car->reg);
	int value_2 = get_value(car->params[2].integer, car->types[2], car->reg);


	int index = car->position + value_1 + value_2; // only T_DIR

	int reg_num = car->params[3].integer;
	if (reg_num < 1 || reg_num > 16)
		return ;
	

	car->reg[reg_num].bytes[3] = war->map[index + 0]->value;
	car->reg[reg_num].bytes[2] = war->map[index + 1]->value;
	car->reg[reg_num].bytes[1] = war->map[index + 2]->value;
	car->reg[reg_num].bytes[0] = war->map[index + 3]->value;

	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | lldi %d %d r%d\n", car->number, value_1, value_2, reg_num);
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
			value_1,
			value_2,
			value_1 + value_2,
			index);
	}


}

void	op_st(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int index = car->position + car->params[2].integer % IDX_MOD; // T_IND
	if (index < 0)
		index += MEM_SIZE;
	// ft_printf("%d\n", index);

	int reg_num = car->params[1].integer;

	// show_union(car->reg[reg_num]);

	if (reg_num < 1 || reg_num > 16)
	{
		// ft_printf("Incorrect reg\n");
		return ;
	}

	if (car->types[2] == T_IND)
	{
		war->map[(index + 0) % MEM_SIZE]->value = car->reg[reg_num].bytes[3];
		war->map[(index + 1) % MEM_SIZE]->value = car->reg[reg_num].bytes[2];
		war->map[(index + 2) % MEM_SIZE]->value = car->reg[reg_num].bytes[1];
		war->map[(index + 3) % MEM_SIZE]->value = car->reg[reg_num].bytes[0];
	}

	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | st r%d %d\n", car->number, reg_num, car->params[2].integer);
	}
}





void	op_sti(t_carriage *car, t_war *war)
{
	int i;


	int reg_num = car->params[1].integer;
	int value_2 = get_value(car->params[2].integer, car->types[2], car->reg);
	int value_3 = get_value(car->params[3].integer, car->types[3], car->reg);

	if (reg_num < 1 || reg_num > 16)
		return ;

	int index = car->position + (value_2 + value_3) % IDX_MOD;
	if (index < 0)
		index += MEM_SIZE;


	war->map[(index + 3) % MEM_SIZE]->value = car->reg[reg_num].bytes[0];
	war->map[(index + 2) % MEM_SIZE]->value = car->reg[reg_num].bytes[1];
	war->map[(index + 1) % MEM_SIZE]->value = car->reg[reg_num].bytes[2];
	war->map[(index + 0) % MEM_SIZE]->value = car->reg[reg_num].bytes[3];

	i = -1;
	while (++i < 4)
	{
		
		war->map[(index + i) % MEM_SIZE]->cycles_bold = war->cycle;
		war->map[(index + i) % MEM_SIZE]->color = car->creator->number;
	}
	
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | sti r%d %d %d\n", car->number, reg_num, value_2, value_3);
		ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
			value_2, value_3, value_2 + value_3, car->position + (value_2 + value_3) % IDX_MOD);
	}

}


void	op_fork(t_carriage *car, t_war *war)
{
	t_carriage *new = create_carriage(0, 0, war, car->creator);
	new->position = car->position + car->params[1].integer % IDX_MOD;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | fork %d (%d)\n", car->number, car->params[1].integer, car->position + car->params[1].integer % IDX_MOD);
	}
}

void	op_lfork(t_carriage *car, t_war *war)
{
	t_carriage *new = create_carriage(0, 0, war, car->creator);
	new->position = car->position + car->params[1].integer;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | lfork %d (%d)\n", car->number, car->params[1].integer, new->position);
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
	{
		jump_status = "FAILED";
	}

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | zjmp %d %s\n", car->number, car->params[1].integer, jump_status);
	}
	
}

void	op_add(t_carriage *car, t_war *war)
{
	car->reg[car->params[3].integer].integer = car->reg[car->params[1].integer].integer + car->reg[car->params[2].integer].integer;
	if (car->reg[car->params[3].integer].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | add r%d r%d r%d\n", car->number,
			car->params[1].integer, car->params[2].integer, car->params[3].integer);
	}
	
}

void	op_sub(t_carriage *car, t_war *war)
{
	car->reg[car->params[3].integer].integer = car->reg[car->params[1].integer].integer - car->reg[car->params[2].integer].integer;
	if (car->reg[car->params[3].integer].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | sub r%d r%d r%d\n", car->number,
			car->params[1].integer, car->params[2].integer, car->params[3].integer);
	}
	
}

void	op_and(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1 = get_value(car->params[1].integer, car->types[1], car->reg);
	int value_2 = get_value(car->params[2].integer, car->types[2], car->reg);

	int reg_num = car->params[3].integer;

	if (reg_num < 1 || reg_num > 16)
		return ;

	car->reg[reg_num].integer = value_1 & value_2;
	if (car->reg[reg_num].integer == 0)
		car->carry = true;
	else
		car->carry = false;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | and %d %d r%d\n", car->number,
			value_1, value_2, reg_num);
	}
	
}

void	op_xor(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1 = get_value(car->params[1].integer, car->types[1], car->reg);
	int value_2 = get_value(car->params[2].integer, car->types[2], car->reg);

	car->reg[car->params[3].integer].integer = value_1 ^ value_2;
	if (car->reg[car->params[3].integer].integer == 0)
		car->carry = true;
	else
		car->carry = false;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | xor %d %d r%d\n", car->number,
			value_1, value_2, car->params[3].integer);
	}
	
}

void	op_new(t_carriage *car, t_war *war)
{


	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | \n", car->number);
	}
	
}

t_op		op_tab[] =  // [17]
{
//   name       args                                                                      code cycles codage label
	{ "live",	1,	{					T_DIR,						0,				0	},	 1,	  10,	0,		0,	 &op_live	},
	{   "ld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	 2,	   5,	1,		0,	   &op_ld	},
	{   "st",	2,	{					T_REG,			T_IND | T_REG,				0	},	 3,	   5,	1,		0,	   &op_st	},
	{  "add",	3,	{					T_REG,					T_REG,			T_REG	},	 4,	  10,	1,		0,	  &op_add	},
	{  "sub",	3,	{					T_REG,					T_REG,			T_REG	},	 5,	  10,	1,		0,	  &op_sub	},
	{  "and",	3,	{	T_REG | T_IND | T_DIR,	T_REG | T_IND | T_DIR,			T_REG	},	 6,	   6,	1,		0,	  &op_and	},
	{  "xor",	3,	{	T_REG | T_IND | T_DIR,	T_REG | T_IND | T_DIR,			T_REG	},	 8,	   6,	1,		0,	  &op_xor	},
	{ "zjmp",	1,	{					T_DIR,						0,				0	},	 9,	  20,	0,		1,	 &op_zjmp	},
	{  "ldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	10,	  25,	1,		1,	  &op_ldi	},
	{  "sti",	3,	{					T_REG,	T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	  25,	1,		1,	  &op_sti	},
	{ "fork",	1,	{					T_DIR,						0,				0	},	12,	 800,	0,		1,	 &op_fork	},
	{ "lldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	14,	  50,	1,		1,	 &op_lldi	},
	{"lfork",	1,	{					T_DIR,						0,				0	},	15,	1000,	0,		1,	&op_lfork	},
	{      0,	0,	{						0,						0,				0	},	 0,	   0,	0,		0,			0	}
};