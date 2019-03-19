/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 14:05:46 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/09 14:05:47 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdio.h>

void	ft_arg_error(char *message)
{
	ft_putendl_fd(message, 2);
	system("leaks asm");
	exit(1);
}

void	ft_error(t_asm *asm_parsing, char *message)
{
	// ft_putendl_fd(asm_parsing->row, 2);
	// ft_putendl_fd(asm_parsing->symbol, 2);
	printf("asm_parsing->row = %d\n", asm_parsing->row);
	printf("asm_parsing->symbol = %d\n", asm_parsing->symbol);
	ft_putendl_fd(message, 2);
	system("leaks asm");
	exit(1);
}

void	error_word(t_asm *asm_parsing, char *message)
{
	printf("Not valid word on row %d \"%s\"\n", asm_parsing->row, message);
	system("leaks asm");
	exit(1);
}

// delete this
void	print_asm_structure(t_asm *asm_parsing)
{
	printf("===========  ASM_PARSING  ==========\n");
	printf("asm_parsing->name_champ = %s\n", asm_parsing->name_champ);
	printf("asm_parsing->comment = %s\n", asm_parsing->comment);
	printf("asm_parsing->row = %d\n", asm_parsing->row);
	printf("asm_parsing->symbol = %d\n", asm_parsing->symbol);
}

t_asm	*init_asm(int fd)
{
	t_asm	*asm_parsing;

	if (!(asm_parsing = (t_asm *)malloc(sizeof(t_asm))))
		ft_arg_error("Can't allocate memory");
	asm_parsing->fd = fd;
	asm_parsing->name_champ = NULL;
	asm_parsing->comment = NULL;
	asm_parsing->row = 0;
	asm_parsing->symbol = 0;
	asm_parsing->words = NULL;
	return (asm_parsing);
}

void	define_cmd(t_asm *asm_parsing, char *line)
{
	
}

void	ignore_comment(t_asm *asm_parsing, char *line)
{
	if (line[asm_parsing->symbol] == COMMENT_CHAR)
		while (line[asm_parsing->symbol])
			asm_parsing->symbol++;
}

char	*take_word(int end , char *line, int start)
{
	char	*temp;
	
	temp = ft_strsub(line, start, end - start);
	// printf("END - START = %d\n", end - start);
	return (temp);
}

void	print_list(t_asm *asm_parsing)
{
	t_word	*current;

	current = asm_parsing->words;
	while (current != NULL)
	{
		printf("current->name = %s\n", current->name);
		printf("current->word_type = %s\n", test[current->word_type - 1]);
		printf("======================================\n");
		current = current->next;
	}
}

int		check_for_instruction(char *name)
{
	int	i;

	i = 0;
	while (i < 17)
	{
		if (ft_strequ(name, g_op_tab[i].name))
			return (1);
		i++;
	}
	return (0);
}

int		check_for_register(t_asm *asm_parsing, char *name)
{
	int	i;

	i = 0;
	if (name[i] && name[0] == 'r')
	{
		i++;
		while (name[i])
		{
			if (ft_isdigit(name[i]))
				i++;
			else
				return (0);
		}
		if ((ft_strlen(name) > 1) && (ft_strlen(name) < 4))
			return (1);
	}
	return (0);
}

int		check_for_number(t_asm *asm_parsing, char *name)
{
	int i;

	i = 0;
	if (name[0] == '-')
		i++;
	while (name[i])
	{
		if (ft_isdigit(name[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	cut_command(t_asm *asm_parsing, char *line)
{
	int		start;
	char	*substring;

	start = asm_parsing->symbol++;
	while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		asm_parsing->symbol++;
	substring = take_word(asm_parsing->symbol, line, start);
	// printf("CUT = %s\n", substring);
	add_word_to_list(asm_parsing, create_word(substring, COMMAND));
}

// void	cut_doubles(t_asm *asm_parsing, char *line)
// {
// 	int		start;
// 	char	*substring;

// 	start = asm_parsing->symbol++;
// 	while (line[asm_parsing->symbol] && line[asm_parsing->symbol] != '"')
// 		asm_parsing->symbol++;
// 	substring = take_word(++asm_parsing->symbol, line, start);
// 	add_word_to_list(asm_parsing, create_word(substring, DOUBLES));
// 	// printf("CUT = %s\n", substring);
// }

void	parse_word(t_asm *asm_parsing, char *line)
{
	int	start;
	char	*substring;
	char	*temp_join;

	if (line[asm_parsing->symbol] == '.')
	{
		// start = asm_parsing->symbol++;
		// while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		// 	asm_parsing->symbol++;
		// substring = take_word(asm_parsing->symbol, line, start);
		// // printf("CUT = %s\n", substring);
		// add_word_to_list(asm_parsing, create_word(substring, COMMAND));
		cut_command(asm_parsing, line);
	}
	else if (line[asm_parsing->symbol] == '"')
	{
		start = asm_parsing->symbol++;
		while (line[asm_parsing->symbol] && line[asm_parsing->symbol] != '"')
			asm_parsing->symbol++;
		substring = take_word(++asm_parsing->symbol, line, start);
		add_word_to_list(asm_parsing, create_word(substring, DOUBLES));
		// printf("CUT = %s\n", substring);
	}
	else if (line[asm_parsing->symbol] == LABEL_CHAR)
	{
		start = asm_parsing->symbol++;
		while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
			asm_parsing->symbol++;
		substring = take_word(asm_parsing->symbol, line, start);
		if (ft_strlen(substring) > 1)
			add_word_to_list(asm_parsing, create_word(substring, INDIRECT));
		else
			ft_error(asm_parsing, "Lexical error");
	}
	else if (line[asm_parsing->symbol] == DIRECT_CHAR)
	{
		start = asm_parsing->symbol++;
		if (line[asm_parsing->symbol] == LABEL_CHAR || line[asm_parsing->symbol] == '-')
			asm_parsing->symbol++;
		while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
			asm_parsing->symbol++;
		substring = take_word(asm_parsing->symbol, line, start);
		// if (ft_strlen(substring) == 1)
		// 	ft_error(asm_parsing, "DICK");
		add_word_to_list(asm_parsing, create_word(substring, DIRECT_ARG));
	}
	else if (line[asm_parsing->symbol] == ',')
	{
		start = asm_parsing->symbol++;
		substring = take_word(asm_parsing->symbol, line, start);
		add_word_to_list(asm_parsing, create_word(substring, SEPARATOR));
	}
	else if (line[asm_parsing->symbol])
	{
		// printf("symbol = %c\n", line[asm_parsing->symbol]);
		// printf("asm_parsing->row = %d | asm_parsing->symbol = %d\n", asm_parsing->row, asm_parsing->symbol);
		start = asm_parsing->symbol;
		if (line[asm_parsing->symbol] == '-')
			asm_parsing->symbol++;
		if (!ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
			ft_error(asm_parsing, "Lexical errorss");
		while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		{
			// printf("%d->%d ", asm_parsing->symbol, line[asm_parsing->symbol]);
			asm_parsing->symbol++;
		}
		// printf("\n");
		if (line[asm_parsing->symbol] == LABEL_CHAR)
		{
			// ft_strlen(substring = take_word(++asm_parsing->symbol, line, start)) > 1 ? 1 : ft_error("Lexical error");
			substring = take_word(++asm_parsing->symbol, line, start);
			// if label contains not only ':'
			if (ft_strlen(substring) > 1)
				add_word_to_list(asm_parsing, create_word(substring, LABEL));
			else
				ft_error(asm_parsing, "Lexical error");
		}
		else
		{
			substring = take_word(asm_parsing->symbol, line, start);
			if (check_for_instruction(substring))
				add_word_to_list(asm_parsing, create_word(substring, INSTRUCTION));
			else if (check_for_register(asm_parsing, substring))
				add_word_to_list(asm_parsing, create_word(substring, REGISTER));
			else if (check_for_number(asm_parsing, substring))
				add_word_to_list(asm_parsing, create_word(substring, INDIRECT));
			else
				error_word(asm_parsing, substring);
		}
	}
}

void	parse_line(t_asm *asm_parsing, char *line)
{
	while (line[asm_parsing->symbol])
	{
		while (ft_isspace(line[asm_parsing->symbol]))
			asm_parsing->symbol++;
		ignore_comment(asm_parsing, line);
		parse_word(asm_parsing, line);
	}
}

void	interpreter(const char *filename)
{
	int		fd;
	char	*line;
	int		ret;
	t_asm	*asm_parsing;

	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd);
	// print_asm_structure(asm_parsing);
	while ((ret = get_next_line(fd, &line)) > 0 && line != NULL)
	{
		asm_parsing->row++;
		// printf("Line = %s\n", line);
		parse_line(asm_parsing, line);
		asm_parsing->symbol = 0;
		ft_strdel(&line);
		add_word_to_list(asm_parsing, create_word(ft_strdup("next line"), NEXT_LINE));
	}
	print_list(asm_parsing);
	print_asm_structure(asm_parsing);
	if (ret == -1)
		ft_arg_error("You can't open directory");
	free_list(asm_parsing);
	free(asm_parsing);
}

int main(int argc, char const *argv[])
{
	char *extention;

	if (argc != 2)
		ft_arg_error("Usage: ./asm champion.s");
	extention = ft_strdup(ft_strrchr(argv[1], '.'));
	if (!ft_strequ(extention, ".s"))
		ft_arg_error("Extention of the file must be .s");
	if (ft_strlen(argv[1]) < 3)
		ft_arg_error("Missed filename");
	interpreter(argv[1]);

	system("leaks asm");
	return (0);
}