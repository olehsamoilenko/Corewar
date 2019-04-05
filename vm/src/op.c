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

// int		get_value_old(int param, int type, union converter *reg, t_war *war, int index)
// {
// 	if (type == T_DIR)
// 		return (param);
// 	else if (type == T_REG)
// 		return (reg[param].integer);
// 	else if (type == T_IND)
// 	{
// 		union converter num;
// 		num.bytes[3] = war->map[index + 0]->value;
// 		num.bytes[2] = war->map[index + 1]->value;
// 		num.bytes[1] = war->map[index + 2]->value;
// 		num.bytes[0] = war->map[index + 3]->value;
// 		return (num.integer);
// 	}
// 	return (0);
// }

t_bool		get_value(t_carriage *car, int num, t_war *war, int index, int *res)
{
	if (car->types[num] == T_DIR)
	{

		*res = car->params[num].integer;

	}
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
		index = (index + MEM_SIZE) % MEM_SIZE;
		num.bytes[3] = war->map[(index + 0) % MEM_SIZE]->value;
		num.bytes[2] = war->map[(index + 1) % MEM_SIZE]->value;
		num.bytes[1] = war->map[(index + 2) % MEM_SIZE]->value;
		num.bytes[0] = war->map[(index + 3) % MEM_SIZE]->value;
		*res = num.integer;
	}
	return (true);
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

	// t_bool sucess = true;


	int reg_num = car->params[2].integer;

	if (reg_num < 1 || reg_num > 16)
		return ;

	get_value(car, 1, war,
		car->position + car->params[1].integer % IDX_MOD, &car->reg[reg_num].integer);



	if (car->reg[reg_num].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | ld %d r%d\n", car->number, car->reg[reg_num].integer, car->params[2].integer);
	}

}

void	op_lld(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	// t_bool sucess = true;


	int reg_num = car->params[2].integer;

	if (reg_num < 1 || reg_num > 16)
		return ;

	get_value(car, 1, war,
		car->position + car->params[1].integer, &car->reg[reg_num].integer);



	if (car->reg[reg_num].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | ld %d r%d\n", car->number, car->reg[reg_num].integer, car->params[2].integer);
	}

}




void	op_ldi(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1; get_value(car, 1, war, 0, &value_1);
	int value_2; get_value(car, 2, war, 0, &value_2);

	int index = car->position + (value_1 + value_2) % IDX_MOD; // only T_DIR
	

	int reg_num = car->params[3].integer;
	if (reg_num < 1 || reg_num > 16)
		return ;
		

	car->reg[reg_num].bytes[3] = war->map[index + 0]->value;
	car->reg[reg_num].bytes[2] = war->map[index + 1]->value;
	car->reg[reg_num].bytes[1] = war->map[index + 2]->value;
	car->reg[reg_num].bytes[0] = war->map[index + 3]->value;



	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | ldi %d %d r%d\n", car->number, value_1, value_2, reg_num);
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

	int value_1; get_value(car, 1, war, 0, &value_1);
	int value_2; get_value(car, 2, war, 0, &value_2);


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

	int r1 = car->params[1].integer;

	

	if (r1 < 1 || r1 > 16)
	{
		// ft_printf("Incorrect reg\n");
		return ;
	}

	if (car->types[2] == T_IND)
	{
		war->map[(index + 0) % MEM_SIZE]->value = car->reg[r1].bytes[3];
		war->map[(index + 1) % MEM_SIZE]->value = car->reg[r1].bytes[2];
		war->map[(index + 2) % MEM_SIZE]->value = car->reg[r1].bytes[1];
		war->map[(index + 3) % MEM_SIZE]->value = car->reg[r1].bytes[0];
	}
	else if (car->types[2] == T_REG)
	{
		int r2 = car->params[2].integer;
		if (r2 < 1 || r2 > 16)
			return ;
		car->reg[r2].bytes[0] = car->reg[r1].bytes[0];
		car->reg[r2].bytes[1] = car->reg[r1].bytes[1];
		car->reg[r2].bytes[2] = car->reg[r1].bytes[2];
		car->reg[r2].bytes[3] = car->reg[r1].bytes[3];
		// show_union(car->reg[r1]);
		// show_union(car->reg[r2]);
	}



	


	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | st r%d %d\n", car->number, r1, car->params[2].integer);
	}
}



void	op_sti(t_carriage *car, t_war *war)
{
	int i;


	if (war->cycle == 18080)
	{
		// show_args(war, car);
		// ft_printf("%d\n", car->codage);
		// reg_info(car->reg, war);
		// (char)245 ;
		// (unsigned char)4110417920

	}

	int r1 = car->params[1].integer;
	int value_2;
	int value_3;
	if (!get_value(car, 2, war, 0, &value_2) || !get_value(car, 3, war, 0, &value_3))
		return ;

	if (r1 < 1 || r1 > 16)
		return ;

	int index = car->position + (value_2 + value_3) % IDX_MOD;
	if (index < 0)
		index += MEM_SIZE;

	// if (car->types[2] == T_REG)
	// {
		war->map[(index + 3) % MEM_SIZE]->value = car->reg[r1].bytes[0];
		war->map[(index + 2) % MEM_SIZE]->value = car->reg[r1].bytes[1];
		war->map[(index + 1) % MEM_SIZE]->value = car->reg[r1].bytes[2];
		war->map[(index + 0) % MEM_SIZE]->value = car->reg[r1].bytes[3];

		// if (war->cycle == 7365)
		// 	ft_printf("%x\n", car->reg[r1].bytes[0]);
	// }
	// else if (car->types[2] == T_REG)
	// {
	// 	ft_printf("ST ARG 2 T_REG\n");
	// (char)62723
	// 	exit(0);
	// }

	i = -1;
	while (++i < 4)
	{
		
		war->map[(index + i) % MEM_SIZE]->cycles_bold = war->cycle;
		war->map[(index + i) % MEM_SIZE]->color = car->creator->number;
	}
	
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | sti r%d %d %d\n", car->number, r1, value_2, value_3);
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
	new->position = (car->position + car->params[1].integer + MEM_SIZE) % MEM_SIZE;
	push_carriage(new, &war->carriages);
	int i = -1;
	while (++i <= REG_NUMBER)
		new->reg[i] = car->reg[i];
	new->last_live = car->last_live;
	new->carry = car->carry;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | lfork %d (%d)\n", car->number, car->params[1].integer,
			car->position + car->params[1].integer);
	}
}

void	op_zjmp(t_carriage *car, t_war *war)
{
	char *jump_status;
	
	// if (war->cycle == 17525)
	// 	show_args(war, car);

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
	int r1 = car->params[1].integer;
	int r2 = car->params[2].integer;
	int r3 = car->params[3].integer;
	if (r1 < 1 || r1 > 16 || r2 < 1 || r2 > 16 || r3 < 1 || r3 > 16)
		return ;
	car->reg[r3].integer = car->reg[r1].integer + car->reg[r2].integer;
	if (car->reg[r3].integer == 0)
		car->carry = true;
	else
		car->carry = false;
	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | add r%d r%d r%d\n", car->number, r1, r2, r3);
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

	int value_1; get_value(car, 1, war, 0, &value_1);
	int value_2; get_value(car, 2, war, 0, &value_2);

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

void	op_or(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1; get_value(car, 1, war, 0, &value_1);
	int value_2; get_value(car, 2, war, 0, &value_2);

	int reg_num = car->params[3].integer;

	if (reg_num < 1 || reg_num > 16)
		return ;

	car->reg[reg_num].integer = value_1 | value_2;
	if (car->reg[reg_num].integer == 0)
		car->carry = true;
	else
		car->carry = false;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | or %d %d r%d\n", car->number,
			value_1, value_2, reg_num);
	}
	
}

void	op_xor(t_carriage *car, t_war *war)
{
	// show_args(war, car);

	int value_1;
	int value_2;
	int r3 = car->params[3].integer;
	if (!get_value(car, 1, war, 0, &value_1) || !get_value(car, 2, war, 0, &value_2)
		|| r3 < 1 || r3 > 16)
		return ;
	

	car->reg[r3].integer = value_1 ^ value_2;

	if (car->reg[r3].integer == 0)
		car->carry = true;
	else
		car->carry = false;

	// verbose
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("P%5d | xor %d %d r%d\n", car->number,
			value_1, value_2, r3);
	}
	
}

void	op_aff(t_carriage *car, t_war *war)
{

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
	{ "live",	1,	{					T_DIR,						0,				0	},	 1,	  10,	0,	0,	 &op_live	},
	{   "ld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	 2,	   5,	1,	0,	   &op_ld	},
	{   "st",	2,	{					T_REG,			T_IND | T_REG,				0	},	 3,	   5,	1,	0,	   &op_st	},
	{  "add",	3,	{					T_REG,					T_REG,			T_REG	},	 4,	  10,	1,	0,	  &op_add	},
	{  "sub",	3,	{					T_REG,					T_REG,			T_REG	},	 5,	  10,	1,	0,	  &op_sub	},
	{  "and",	3,	{	T_REG | T_IND | T_DIR,	T_REG | T_IND | T_DIR,			T_REG	},	 6,	   6,	1,	0,	  &op_and	},
	{   "or",	3,	{	T_REG | T_IND | T_DIR,	T_REG | T_IND | T_DIR,			T_REG	},	 7,	   6,	1,	0,	   &op_or	},
	{  "xor",	3,	{	T_REG | T_IND | T_DIR,	T_REG | T_IND | T_DIR,			T_REG	},	 8,	   6,	1,	0,	  &op_xor	},
	{ "zjmp",	1,	{					T_DIR,						0,				0	},	 9,	  20,	0,	1,	 &op_zjmp	},
	{  "ldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	10,	  25,	1,	1,	  &op_ldi	},
	{  "sti",	3,	{					T_REG,	T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	  25,	1,	1,	  &op_sti	},
	{ "fork",	1,	{					T_DIR,						0,				0	},	12,	 800,	0,	1,	 &op_fork	},
	{  "lld",	2,	{			T_DIR | T_IND,					T_REG,				0	},	13,	  10,	1,	0,	  &op_lld	},
	{ "lldi",	3,	{	T_REG | T_DIR | T_IND,			T_DIR | T_REG,			T_REG	},	14,	  50,	1,	1,	 &op_lldi	},
	{"lfork",	1,	{					T_DIR,						0,				0	},	15,	1000,	0,	1,	&op_lfork	},
	{  "aff",	1,	{					T_REG,						0,				0	},	16,	   2,	1,	0,	  &op_aff	},
	{      0,	0,	{						0,						0,				0	},	 0,	   0,	0,	0,			0	}
};