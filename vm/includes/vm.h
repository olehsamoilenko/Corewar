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

# include <time.h>

# include "op.h"
# include <stdio.h> // trash
# include <curses.h>
# include <fcntl.h>

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_S 115

# define KEY_W 119
# define KEY_E 101

union converter // u_converter ?
{
	int				integer; // unsigned ?
	unsigned char	bytes[REG_SIZE];
};

typedef struct	s_champion
{
	char				*file;
	int					number;
	header_t			*header;
	int					last_live;
	int					lives_cur_period;
}				t_champion;

typedef struct	s_map_cell // fix
{
	unsigned char		value;
	int					color;
	int					cycles_live;
	int					cycles_bold;
}				t_mem_cell;

typedef struct	s_carriage
{
	int					number;
	t_champion			*creator;
	int					position; // PC, program counter
	union converter		reg[REG_NUMBER + 1]; // r1 is reg[1] r0 is unused

	struct s_op			*op;
	int					cooldown;
	t_bool				carry:1;
	int					last_live;
	struct s_carriage	*next;

	int					codage;
	int					types[MAX_ARGS_NUMBER]; // 0 is unused
	int					sizes[MAX_ARGS_NUMBER]; // 0 is unused
	union converter		params[MAX_ARGS_NUMBER]; // 0 is unused

	t_bool				args_ok:1;

}				t_carriage;

typedef struct	s_war
{
	WINDOW				*win_mem;
	WINDOW				*win_info;

	t_mem_cell			*map[MEM_SIZE];
	t_champion			*champs[MAX_PLAYERS + 1]; // champs[4] is NULL

	t_carriage			*carriages;
	int					processes_counter; // for number of carriage
	int					processes_amount; // for visual

	int					cycle; // current cycle
	int					cycles_after_check;
	int					cycles_to_die;
	int					checks;

	t_champion			*last_live; // if anybody ?
	int					all_lives;

	t_bool				flag_verbose:1;
	int					flag_dump; // -1 if not defined
	// t_bool				flag_aff:1;

	t_bool				flag_visual:1;
	t_bool				flag_run:1; // for stop game
	int					cycles_in_second; // need per_sec
	int					last_print;

	int					last_line; // for winner
	float				time;
	
}				t_war;

typedef struct	s_op
{
	char				*name;
	int					args;
	int					args_type[3];
	int					code;
	int					cooldown;
	t_bool				codage:1;
	t_bool				label:1;
	void				(*func)(t_carriage *, t_war *);
}				t_op;


extern t_op op_tab[];

// main
void		error(char *message);

// params
void		parse_params(int argc, char **argv, t_war *war);

// champion
void		parse_champions(t_champion *champs[], t_mem_cell *map[], int mem_delta);
t_champion	*find_champ(int number, t_war *war);

// init
t_war		*init();

// carriage
void		push_carriage(t_carriage *car, t_carriage **list);
t_carriage	*create_carriage(int position, int player, t_war *war, t_champion *creator);
void		show_carriages(t_war *war);

// verbose
void		adv(t_war *war, t_op *op, int instr_len, t_carriage *car);
void		dump(t_war *war);

// curses
void		print_info(t_war *war);
void		print_memory(t_war *war);
void		init_curses(t_war *war);
void		over_over(t_war *war);
void		over_curses(t_war *war);

// tmp
void		show_args(t_war *war, t_carriage *car);
void		curriage_info(t_carriage *car, t_war *war);
char		*define_type(int type);
void		show_union(union converter a);
void		reg_info(union converter *reg, t_war *war);
void		print_champions(t_champion *champs[]);

#endif
