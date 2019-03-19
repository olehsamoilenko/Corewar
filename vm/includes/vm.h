/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 11:39:25 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/02 11:39:26 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "libft.h"
# include "op.h"
# include <stdio.h> // trash




// reversed bytes ??? [ 17 00 00 00 ] is 23
union magic_header
{
	int					hex;
	unsigned char		bytes[4];
};

typedef struct	s_champion
{
	char				*file;
	header_t			*header;
}				t_champion;

typedef struct	s_map_cell
{
	unsigned char		value;
	int					color;
}				t_mem_cell;

typedef struct	s_carriage
{
	int					position;
	int					op_code;
	int					cooldown;
	int					color;
	struct s_carriage	*next;
}				t_carriage;

typedef struct	s_war
{
	t_mem_cell			*map[MEM_SIZE];
	t_champion			*champs[5];
	t_carriage			*carriages;

}				t_war;

typedef struct	s_op
{
	char				*name;
	int					args;
	int					args_type[3];
	int					code;
	int					cooldown;
}				t_op;


t_op		op_tab[] =  // [17]
{
	{"ld",	2,	{T_DIR | T_IND,	T_REG,					0				},	2,	5	},
	{"st",	2,	{T_REG,			T_IND | T_REG,			0				},	3,	5	},
	{"sti",	3,	{T_REG,			T_REG | T_DIR | T_IND,	T_DIR | T_REG	},	11,	25	},
	{0,		0,	0,															0,	0	}
};


#endif
