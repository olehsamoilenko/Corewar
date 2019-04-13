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
# include <fcntl.h>
# include <time.h>
# include <curses.h>

# define KEY_ESC 27
# define KEY_SPACE 32
# define KEY_S 115
# define KEY_W 119
# define KEY_E 101

# define ERR_MANY_CHAMPS "Virtual machine allows up to 4 champions"
# define ERR_NO_CHAMPS "There are any champions present"
# define ERR_N_NUMBER "Flag -n needs a number in range of 1 to 4"
# define ERR_N_CHAMP "Flag -n needs a champion"
# define ERR_SAME_N "Duplication of champion's number is forbidden"
# define ERR_CHAMP_FORMAT "Champion format must be 'name.cor'"
# define ERR_BIG_N "Champion's number exceeds their amount"
# define ERR_DUMP "Flag -dump needs a positive number"
# define ERR_VER_VIS "Combination of -verbose and -visual is forbidden"
# define ERR_OPEN_CHAMP "Can't open the champion"
# define ERR_MAGIC_HEADER "Magic header is incorrect"
# define ERR_NULL_AFTER_NAME "There aren't empty octets after the name"
# define ERR_NULL_AFTER_COMMENT "There aren't empty octets after the comment"
# define ERR_BIG_CHAMP "Too big champion"
# define ERR_SIZE_DIFFERS "File has incorrect size of code"
# define ERR_SMALL_FILE "File is too small to be a champion"

typedef union	u_converter
{
	int					integer;
	unsigned char		bytes[REG_SIZE];
}				t_converter;

typedef struct	s_champion
{
	char				*file;
	int					number;
	t_header			*header;
	int					last_live;
	int					lives_cur_period;
}				t_champion;

typedef struct	s_map_cell
{
	unsigned char		value;
	int					color;
	int					cycles_live;
	int					cycles_bold;
}				t_map_cell;

typedef struct	s_carriage
{
	int					number;
	t_champion			*creator;
	int					position;
	t_converter			reg[REG_NUMBER + 1];
	struct s_op			*op;
	int					cooldown;
	t_bool				carry:1;
	int					last_live;
	int					codage;
	int					type[MAX_ARGS_NUMBER];
	int					size[MAX_ARGS_NUMBER];
	t_converter			param[MAX_ARGS_NUMBER];
	int					value[MAX_ARGS_NUMBER];
	struct s_carriage	*next;

}				t_carriage;

typedef struct	s_war
{
	t_map_cell			*map[MEM_SIZE];
	t_champion			*champs[MAX_PLAYERS + 1];
	int					amount_champs;
	t_carriage			*carriages;
	int					processes_counter;
	int					cycle;
	int					cycles_after_check;
	int					cycles_to_die;
	int					checks;
	t_champion			*last_live;
	int					all_lives;
	t_bool				flag_verbose:1;
	int					flag_dump;
	t_bool				flag_visual:1;
	t_bool				flag_run:1;
	WINDOW				*win_mem;
	WINDOW				*win_info;
	int					cycles_in_second;
	int					last_print;
	int					last_line;
	int					processes_amount;
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

void			error(char *message);
void			usage(void);
void			parse_params(int argc, char **argv, t_war *war);
void			parse_champions(t_war *war);
t_champion		*find_champ(int number, t_war *war);
t_bool			is_champion(char *name);
t_champion		*create_champion(char *file);
void			push_carriage(t_carriage *car, t_carriage **list);
t_carriage		*create_carriage(t_war *war, t_champion *creator);
void			throw_basic_carriages(t_war *war);
void			adv(t_war *war, int instr_len, t_carriage *car);
void			dump(t_war *war);
void			verbose_won(t_war *war);
void			introduce(t_war *war, t_champion **champs, t_bool flag_visual);
void			print_info(t_war *war);
void			print_memory(t_war *war);
void			init_curses(t_war *war);
void			over_over(t_war *war);
void			over_curses(t_war *war);
t_bool			get_value(t_carriage *car, int num, t_war *war, int index);
t_bool			correct_reg(int reg_num);
t_bool			check_carry(int value);
t_converter		get_from_map(t_war *war, int index);
void			throw_on_map(t_converter value, t_war *war, t_carriage *car,
				int index);
void			run_carriages(t_war *war);
void			check(t_war *war);
void			read_comment(int fd, char *comment);
int				read_exec_code_size(int fd);
int				read_null(int fd);
void			read_name(int fd, char *name);
int				read_magic_header(int fd);
void			op_live(t_carriage *car, t_war *war);
void			op_ld(t_carriage *car, t_war *war);
void			op_st(t_carriage *car, t_war *war);
void			op_add(t_carriage *car, t_war *war);
void			op_sub(t_carriage *car, t_war *war);
void			op_and(t_carriage *car, t_war *war);
void			op_or(t_carriage *car, t_war *war);
void			op_xor(t_carriage *car, t_war *war);
void			op_zjmp(t_carriage *car, t_war *war);
void			op_ldi(t_carriage *car, t_war *war);
void			op_sti(t_carriage *car, t_war *war);
void			op_fork(t_carriage *car, t_war *war);
void			op_lld(t_carriage *car, t_war *war);
void			op_lldi(t_carriage *car, t_war *war);
void			op_lfork(t_carriage *car, t_war *war);
void			op_aff(t_carriage *car, t_war *war);
int				get_args(t_carriage *car, t_war *war);
t_bool			next_cycle(t_war *war);

static t_op		g_op_tab[] =
{
	{
		.name = "live",
		.args = 1,
		.args_type = {T_DIR, 0, 0},
		.code = 0x01,
		.cooldown = 10,
		.codage = false,
		.label = false,
		.func = &op_live
	},
	{
		.name = "ld",
		.args = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.code = 0x02,
		.cooldown = 5,
		.codage = true,
		.label = false,
		.func = &op_ld
	},
	{
		.name = "st",
		.args = 2,
		.args_type = {T_REG, T_IND | T_REG, 0},
		.code = 0x03,
		.cooldown = 5,
		.codage = true,
		.label = false,
		.func = &op_st
	},
	{
		.name = "add",
		.args = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.code = 0x04,
		.cooldown = 10,
		.codage = true,
		.label = false,
		.func = &op_add
	},
	{
		.name = "sub",
		.args = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.code = 0x05,
		.cooldown = 10,
		.codage = true,
		.label = false,
		.func = &op_sub
	},
	{
		.name = "and",
		.args = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.code = 0x06,
		.cooldown = 6,
		.codage = true,
		.label = false,
		.func = &op_and
	},
	{
		.name = "or",
		.args = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.code = 0x07,
		.cooldown = 6,
		.codage = true,
		.label = false,
		.func = &op_or
	},
	{
		.name = "xor",
		.args = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.code = 0x08,
		.cooldown = 6,
		.codage = true,
		.label = false,
		.func = &op_xor
	},
	{
		.name = "zjmp",
		.args = 1,
		.args_type = {T_DIR, 0, 0},
		.code = 0x09,
		.cooldown = 20,
		.codage = false,
		.label = true,
		.func = &op_zjmp
	},
	{
		.name = "ldi",
		.args = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.code = 0x0a,
		.cooldown = 25,
		.codage = true,
		.label = true,
		.func = &op_ldi
	},
	{
		.name = "sti",
		.args = 3,
		.args_type = {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},
		.code = 0x0b,
		.cooldown = 25,
		.codage = true,
		.label = true,
		.func = &op_sti
	},
	{
		.name = "fork",
		.args = 1,
		.args_type = {T_DIR, 0, 0},
		.code = 0x0c,
		.cooldown = 800,
		.codage = false,
		.label = true,
		.func = &op_fork
	},
	{
		.name = "lld",
		.args = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.code = 0x0d,
		.cooldown = 10,
		.codage = true,
		.label = false,
		.func = &op_lld
	},
	{
		.name = "lldi",
		.args = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.code = 0x0e,
		.cooldown = 50,
		.codage = true,
		.label = true,
		.func = &op_lldi
	},
	{
		.name = "lfork",
		.args = 1,
		.args_type = {T_DIR, 0, 0},
		.code = 0x0f,
		.cooldown = 1000,
		.codage = false,
		.label = true,
		.func = &op_lfork
	},
	{
		.name = "aff",
		.args = 1,
		.args_type = {T_REG, 0, 0},
		.code = 0x10,
		.cooldown = 2,
		.codage = true,
		.label = false,
		.func = &op_aff
	},
	{
		.name = NULL,
		.args = 0,
		.args_type = {0, 0, 0},
		.code = 0,
		.cooldown = 0,
		.codage = 0,
		.label = 0,
		.func = 0
	}
};

#endif
