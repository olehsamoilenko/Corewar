/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 14:32:04 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/09 14:32:08 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# include "libft.h"
# include "get_next_line.h"

# include "op.h"

# include <fcntl.h>

/*
**	defines for determination kind of word
*/
# define COMMAND 1
# define DOUBLES 2
# define NEXT_LINE 3
# define UNKNOWN 4
# define LABEL 5
# define DIRECT_ARG 6
# define INDIRECT_ARG 7
# define INSTRUCTION 8
# define SEPARATOR 9
# define REGISTER 10
# define END_LINE 11

# define BREAK 33
# define CONTINUE 44

typedef struct	s_op
{
	char			*name;
	int				count_args;
	int				type_args[3];
	int				opcode;
	int				codage_octal;
	int				label_size;
}				t_op;

extern t_op g_op_tab[];

typedef	struct	s_codage
{
	int				instruction;
	int				count_args;
	int				sum;
	unsigned char	var_for_codage;
	char			shift_left;
}				t_codage;

/*
** Structure for labels
** refer - byte on which will refer some label
*/
typedef	struct	s_label
{
	int				refer;
	char			*name;
	struct s_label	*next;
}				t_label;

/*
**	Structure for insruction
*/
typedef	struct	s_word
{
	char			*name;
	unsigned		word_type;
	unsigned		row;
	struct s_word	*next;
}				t_word;

typedef struct	s_instruction
{
	t_word		*args[3];
	int			opcode;

}				t_instruction;

/*
**	Main structure
*/
typedef	struct	s_asm
{
	char		*filename;
	int			fd;
	int			fd_dis;
	char		*name_champ;
	char		*comment;
	unsigned	row;
	unsigned	symbol;
	t_word		*words;
	t_label		*labels;
	char		*byte_code;
	int			position;
	int			pos_labels;
	int			disassembler;
	char		*asm_code;
	int			size_code;

}				t_asm;

/*
** main.c
*/
t_word			*determine_name(t_asm *asm_parsing, t_word *current);
t_word			*determine_comment(t_asm *asm_parsing, t_word *current);
void			print_asm_structure(t_asm *asm_parsing);
t_asm			*init_asm(int fd, const char *filename);

/*
**	list_of_worrds.c
*/
t_word			*create_word(t_asm *asm_parsing, char *name, int type);
void			add_word_to_list(t_asm *asm_parsing, t_word *new_word);

/*
**	list_of_labels.c
*/
t_label			*create_label(char *name, int refer);
void			add_label_to_list(t_asm *asm_parsing, t_label *new_label);

/*
**	parsing_lines.c
*/
void			parse_line(t_asm *asm_parsing, char *line);

/*
**	check_all.c
*/
void			check_label(t_asm *asm_parsing, char *substring,
														char *line, int start);
void			check_for_doubles(t_asm *asm_parsing,
												char *line, char *substring);
void			check_all_lines(t_asm *asm_parsing,
												int ret, int fd, char *line);
int				check_for_register(char *name);
int				check_for_instruction(char *name);

/*
**	error_managment.c
*/
void			ft_arg_error(char *message);
void			ft_error(t_asm *asm_parsing);
void			error_word(t_asm *asm_parsing, char *message);
void			error_word2(t_word *word, char *message);
void			error_dis(char *message);
/*
**	convert_and_write.c
*/
void			write_int_to_byte(t_asm *asm_parsing, int nbr, int size);
void			write_data_to_all(char *all, int start, int data, int size);
void			write_to_file(t_asm *asm_parsing);
/*
**	free_all.c
*/
void			free_all(t_asm *asm_parsing);
int				check_for_disasm(char const *argv[]);

/*
**	process_instruction.c
*/
t_word			*process_instruction(t_asm *asm_parsing, t_word *current);
t_codage		*init_codage(void);

/*
**	process_labels.c
*/
t_word			*process_label(t_asm *asm_parsing, t_word *current);

/*
**	cut_functions.c
*/
void			cut_command(t_asm *asm_parsing, char *line);
void			cut_doubles(t_asm *asm_parsing, char *line, int start);
void			cut_label(t_asm *asm_parsing, char *line);
void			cut_direct(t_asm *asm_parsing, char *line);

/*
**	process_args.c
*/
void			process_direct_arg(t_codage *codage,
							t_instruction *instruction_args, t_word *current);
void			process_indirect_arg(t_codage *codage,
							t_instruction *instruction_args, t_word *current);
void			process_register_arg(t_codage *codage,
							t_instruction *instruction_args, t_word *current);
t_codage		*init_codage(void);

/*
**	write_args.c
*/
void			write_ind_arg(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct);
void			if_exist_label(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct);
void			write_dir_arg(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct);
void			define_arg_type(t_asm *asm_parsing,
			t_instruction *instruction_args, int i, int position_of_instruct);
void			write_args(t_asm *asm_parsing, t_codage *codage,
									t_instruction *instruction_args);

/*
**	help_functions.c
*/
void			ignore_comment(t_asm *asm_parsing, char *line);
char			*take_word(int end, char *line, int start);
int				find_instruction(t_word *current);
t_instruction	*init_instruction_args(void);
int				check_for_number(char *name);

/*
**	determine.c
*/
t_word			*determine_commands(t_asm *asm_parsing);
void			determine_labels(t_asm *asm_parsing, t_word *current);
void			determine_instructions(t_asm *asm_parsing, t_word *current);
t_label			*find_label(t_asm *asm_parsing, t_word *current_label);

/*
**	disassembler.c
*/
void			disassembler(const char *filename);
int				parse_bytecode(t_asm *asm_parsing, unsigned char *all_code,
																	int size);
/*
**	get_args.c
*/
void			get_args_codage(t_asm *asm_parsing, unsigned char *all_code,
										int instruction, unsigned char codage);
void			get_args(t_asm *asm_parsing, unsigned char *all_code,
														int instruction);
/*
**	read_and_parse.c
*/
int				parse_bytecode(t_asm *asm_parsing, unsigned char *all_code,
																	int size);
void			read_comment(t_asm *asm_parsing);
void			read_name(t_asm *asm_parsing);
int				help_dis(t_asm *asm_parsing, int size);
unsigned int	from_bytes_to_int(unsigned char *buf, int size);

#endif
