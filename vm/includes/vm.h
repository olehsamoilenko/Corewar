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
# include <curses.h>
# include <fcntl.h>

// reversed bytes ??? [ 17 00 00 00 ] is 23
union converter // u_converter ?
{
	int					integer; // unsigned ?
	unsigned char		bytes[4];
};

typedef struct	s_champion
{
	char				*file;
	int					number;
	header_t			*header;
}				t_champion;

typedef struct	s_map_cell
{
	unsigned char		value;
	int					color;
	int					cycles_live;
	int					cycles_bold;
}				t_mem_cell;

typedef struct	s_carriage
{
	int					number;
	int					position;
	int					reg[REG_NUMBER + 1]; // r1 is reg[1] r0 is unused
	int					op_code;
	int					cooldown;
	// t_champion			*player;
	// int					process_number;
	int					last_live;
	struct s_carriage	*next;
}				t_carriage;

typedef struct	s_war
{
	WINDOW				*win_mem;
	WINDOW				*win_info;
	WINDOW				*win_getch;
	t_mem_cell			*map[MEM_SIZE];
	t_champion			*champs[4];
	t_carriage			*carriages;
	int					cycle;
	// t_bool				carry;
	t_bool				flag_verbose;
	t_bool				flag_visual;
	int					flag_dump; // -1 if not defined
	int					processes;
}				t_war;

typedef struct	s_op
{
	char				*name;
	int					args;
	int					args_type[3];
	int					code;
	int					cooldown;
	t_bool				codage;
	t_bool				label; // bool
	void				(*func)(int, t_carriage *, t_war *, int *);
}				t_op;

typedef struct	s_instr_params
{
	int					codage;
	// int					amount;
	int					sizes[4]; // 0 is unused
	union converter		params[4]; // 0 is unused
}				t_instr_params;

extern t_op op_tab[];

// main
void	error(char *message);

// params
void	parse_params(int argc, char **argv, t_war *war);

// champion
void	parse_champions(t_champion *champs[], t_mem_cell *map[], int mem_delta);
t_champion	*find_champ(int number, t_war *war);

// init
t_war	*init();
void	init_curses(t_war *war);

// carriage
void		push_carriage(t_carriage *car, t_carriage **list);
t_carriage	*create_carriage(int position, int number, t_war *war);
// t_carriage	*copy_carriage(t_carriage *car);
void		show_carriages(t_war *war);

#endif
