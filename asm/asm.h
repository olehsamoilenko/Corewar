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
# define INDIRECT 7
# define INSTRUCTION 8
# define SEPARATOR 9
# define REGISTER 10

static	char *test[] = 
{
	"COMMAND",
	"DOUBLES",
	"NEXT_LINE",
	"UNKNOWN",
	"LABEL",
	"DIRECT_ARG",
	"INDIRECT",
	"INSTRUCTION",
	"SEPARATOR",
	"REGISTER"
};

typedef struct	s_op
{
	char	*name;
	int		count_args;
}				t_op;

extern t_op g_op_tab[17];


/*
**	Structure for insruction
*/
typedef	struct	s_word
{
	char					*name;
	unsigned				word_type;
	unsigned				row;
	// unsigned				symbol;
	struct	s_word			*next;
}				t_word;

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
**	parsing_lines.c
*/
void	parse_line(t_asm *asm_parsing, char *line);

#endif
