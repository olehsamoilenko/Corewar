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

// DELETE
#include <stdio.h>

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

static	char *test[] = 
{
	"COMMAND",
	"DOUBLES",
	"NEXT_LINE",
	"UNKNOWN",
	"LABEL",
	"DIRECT_ARG",
	"INDIRECT_ARG",
	"INSTRUCTION",
	"SEPARATOR",
	"REGISTER",
	"END_LINE"
};

typedef struct	s_op
{
	char	*name;
	int		count_args;
	int		type_args[3];
	int		opcode;
	int		cycles;
	char	*description;
	int		codage_octal;
	int		label_size;

}				t_op;

extern t_op g_op_tab[];

/*
** Structure for labels
** refer - byte on which will refer some label
*/
typedef	struct	s_label
{
	int		refer;
	char	*name;

	struct	s_label *next;
}				t_label;


/*
**	Structure for insruction
*/
typedef	struct	s_word
{
	char					*name;
	unsigned				word_type;
	unsigned				row;
	struct	s_word			*next;
}				t_word;

typedef struct s_instruction
{
	t_word *args[3];
}				t_instruction;

/*
**	Main structure
*/
typedef	struct	s_asm
{
	int			fd;
	char		*name_champ;
	char		*comment;
	unsigned	row;
	unsigned	symbol;
	t_word		*words;
	t_label		*labels;

	// added
	char *byte_code;
	int	position;

	// position for labels
	int		pos_labels;
}				t_asm;

/*
** main.c
*/
char	*take_word(int end , char *line, int start);
void	ignore_comment(t_asm *asm_parsing, char *line);
int		check_for_register(t_asm *asm_parsing, char *name);
int		check_for_number(t_asm *asm_parsing, char *name);
int		check_for_instruction(char *name);

/*
**	Functions for errors
*/
void	ft_arg_error(char *message);
void	error_word(t_asm *asm_parsing, char *message);
void	ft_error(t_asm *asm_parsing, char *message);

/*
**	list_of_worrds.c
*/
t_word	*create_word(t_asm *asm_parsing, char *name, int type);
void	add_word_to_list(t_asm *asm_parsing, t_word *new_word);
void	free_list(t_asm *asm_parsing);


/*
**	list_of_labels.c
*/
t_label *create_label(char *name, int refer);
void    add_label_to_list(t_asm *asm_parsing, t_label *new_label);

/*
**	parsing_lines.c
*/
void	parse_line(t_asm *asm_parsing, char *line);


// added
char		*ft_itoa_base(uintmax_t n, int base, int flag);
#endif
