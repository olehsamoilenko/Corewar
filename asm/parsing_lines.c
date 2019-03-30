/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 13:57:58 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/20 13:58:01 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	cut_command(t_asm *asm_parsing, char *line)
{
	int		start;
	char	*substring;

	start = asm_parsing->symbol++;
	while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		asm_parsing->symbol++;
	substring = take_word(asm_parsing->symbol, line, start);
	add_word_to_list(asm_parsing, create_word(asm_parsing, substring, COMMAND));
}

void	cut_doubles(t_asm *asm_parsing, char *line, int	start)
{
	char	*substring;
	char	*temp_join;
	char 	*str;


	substring = take_word(asm_parsing->symbol, line, start);
	temp_join = substring;
	while (get_next_line(asm_parsing->fd, &line))
	{
		asm_parsing->row++;
		if (ft_strchr(line, '"'))
			break ;
		temp_join = ft_strjoin(substring, line);
		ft_strdel(&substring);
		substring = temp_join;
		ft_strdel(&line);
	}
	str = take_word(ft_strlen(line) - ft_strlen(ft_strchr(line, '"')) + 1, line, 0);
	char *temp = substring;
	substring = ft_strjoin(temp_join, str);
	ft_strdel(&temp);
	ft_strdel(&str);
	check_for_doubles(asm_parsing, line, substring);
}

void	cut_label(t_asm *asm_parsing, char *line)
{
	int		start;
	char	*substring;

	start = asm_parsing->symbol++;
	while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		asm_parsing->symbol++;
	substring = take_word(asm_parsing->symbol, line, start);
	if (ft_strlen(substring) > 1)
		add_word_to_list(asm_parsing, create_word(asm_parsing, substring, INDIRECT_ARG));
	else
		ft_error(asm_parsing, "Lexical error");
}

void	cut_direct(t_asm *asm_parsing, char *line)
{
	int		start;
	char	*substring;

	start = asm_parsing->symbol++;
	if (line[asm_parsing->symbol] == LABEL_CHAR || line[asm_parsing->symbol] == '-')
		asm_parsing->symbol++;
	while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		asm_parsing->symbol++;
	substring = take_word(asm_parsing->symbol, line, start);
	if (!ft_strchr(substring, ':'))
		if (ft_strchr(substring, '_'))
			error_word(asm_parsing, "Argument contains incorrect symbol");
	if (ft_strlen(substring) < 2)
		error_word(asm_parsing, "Length of argument equals 0");
	add_word_to_list(asm_parsing, create_word(asm_parsing, substring, DIRECT_ARG));
}

void	cut_other(t_asm *asm_parsing, char *line)
{
	int		start;
	char	*substring;

	start = asm_parsing->symbol;
	if (line[asm_parsing->symbol] == '-')
		asm_parsing->symbol++;
	if (!ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		ft_error(asm_parsing, "Lexical errorss");
	while (line[asm_parsing->symbol] && ft_strchr(LABEL_CHARS, line[asm_parsing->symbol]))
		asm_parsing->symbol++;
	if (line[asm_parsing->symbol] == LABEL_CHAR)
		check_label(asm_parsing, substring, line, start);
	else
	{
		substring = take_word(asm_parsing->symbol, line, start);
		if (check_for_instruction(substring))
			add_word_to_list(asm_parsing, create_word(asm_parsing, substring, INSTRUCTION));
		else if (check_for_register(asm_parsing, substring))
			add_word_to_list(asm_parsing, create_word(asm_parsing, substring, REGISTER));
		else if (check_for_number(asm_parsing, substring))
			add_word_to_list(asm_parsing, create_word(asm_parsing, substring, INDIRECT_ARG));
		else
			error_word(asm_parsing, substring);
	}
}

void	cut_doubles_s(t_asm *asm_parsing, char *line, int start, char *substring)
{
	start = asm_parsing->symbol++;
	while (line[asm_parsing->symbol] && line[asm_parsing->symbol] != '"')
		asm_parsing->symbol++;
	if (line[asm_parsing->symbol] != '"')
		cut_doubles(asm_parsing, line, start);
	else
	{
		substring = take_word(++asm_parsing->symbol, line, start);
		add_word_to_list(asm_parsing, create_word(asm_parsing, substring, DOUBLES));
	}
}

void	parse_word(t_asm *asm_parsing, char *line)
{
	int	start;
	char	*substring;

	if (line[asm_parsing->symbol] == '.')
		cut_command(asm_parsing, line);
	else if (line[asm_parsing->symbol] == '"')
		cut_doubles_s(asm_parsing, line, start, substring);
	else if (line[asm_parsing->symbol] == LABEL_CHAR)
		cut_label(asm_parsing, line);
	else if (line[asm_parsing->symbol] == DIRECT_CHAR)
		cut_direct(asm_parsing, line);
	else if (line[asm_parsing->symbol] == ',')
	{
		start = asm_parsing->symbol++;
		substring = take_word(asm_parsing->symbol, line, start);
		add_word_to_list(asm_parsing, create_word(asm_parsing, substring, SEPARATOR));
	}
	else if (line[asm_parsing->symbol])
		cut_other(asm_parsing, line);
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