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

#define KEY_ESC 27
#define KEY_S 115

// reversed bytes ??? [ 17 00 00 00 ] is 23
union converter // u_converter ?
{
	int				integer; // unsigned ?
	unsigned char	bytes[4];
};

typedef struct	s_champion
{
	char				*file;
	int					number;
	header_t			*header;
	int					last_live;
	int					lives_cur_period;
}				t_champion;

typedef struct	s_map_cell
{
	unsigned char		value;
	int					color;
	int					cycles_live;
	int					cycles_bold;
}				t_mem_cell;

// typedef struct s_op t_op;

// typedef struct	s_instr_params
// {
	
// 	// int					amount;
	
	
	
// }				t_instr_params;

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
	int					types[4]; // 0 is unused
	int					sizes[4]; // 0 is unused
	union converter		params[4]; // 0 is unused

	t_bool				args_ok:1;

}				t_carriage;



typedef struct	s_war
{
	WINDOW				*win_mem;
	WINDOW				*win_info;
	WINDOW				*win_getch;

	t_mem_cell			*map[MEM_SIZE];
	t_champion			*champs[4];

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
	t_bool				flag_visual:1;
	int					flag_dump; // -1 if not defined
	t_bool				flag_dev:1; // FOR TESTING !
	
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
void	error(char *message);

// params
void	parse_params(int argc, char **argv, t_war *war);

// champion
void	parse_champions(t_champion *champs[], t_mem_cell *map[], int mem_delta);
t_champion	*find_champ(int number, t_war *war);

// init
t_war	*init();

// carriage
void		push_carriage(t_carriage *car, t_carriage **list);
t_carriage	*create_carriage(int position, int player, t_war *war, t_champion *creator);
void		show_carriages(t_war *war);

// verbose
void	adv(t_war *war, t_op *op, int instr_len, t_carriage *car);
void	dump(t_war *war);

// curses
void	print_memory(t_war *war);
void	init_curses(t_war *war);
void	over_over(t_war *war);
void	over_curses(t_war *war);

// tmp
void	show_args(t_war *war, t_carriage *car);
void	curriage_info(t_carriage *car, t_war *war);
char	*define_type(int type);
void	show_union(union converter a);
void	reg_info(union converter *reg, t_war *war);

#endif
