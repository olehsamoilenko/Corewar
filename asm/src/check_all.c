/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:29:29 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 14:29:30 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	check_label(t_asm *asm_parsing, char *substring, char *line, int start)
{
	int i;

	i = 0;
	substring = take_word(++asm_parsing->symbol, line, start);
	while (i < (int)ft_strlen(substring) - 1)
	{
		if (ft_strchr(LABEL_CHARS, substring[i]) != NULL)
			i++;
		else
			error_word(asm_parsing, substring);
	}
	if (ft_strlen(substring) > 1)
		add_word_to_list(asm_parsing, create_word(asm_parsing, substring,
																	LABEL));
	else
		ft_error(asm_parsing);
}

void	check_for_doubles(t_asm *asm_parsing, char *line, char *substring)
{
	int pos;

	pos = ft_strlen(line) - ft_strlen(ft_strchr(line, '"')) + 1;
	while (ft_strlen(line) > 0 && line[pos])
	{
		if (ft_isspace(line[pos]))
			pos++;
		else if (line[pos] == '#')
			break ;
		else
			error_word(asm_parsing, &line[pos]);
	}
	if (ft_strchr(line, '"') == NULL)
		error_word(asm_parsing, "ERROR");
	ft_strdel(&line);
	add_word_to_list(asm_parsing, create_word(asm_parsing, substring, DOUBLES));
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

int		check_for_register(char *name)
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

void	check_all_lines(t_asm *asm_parsing, int ret, int fd, char *line)
{
	while ((ret = get_next_line(fd, &line)) > 0 && line != NULL)
	{
		asm_parsing->row++;
		parse_line(asm_parsing, line);
		asm_parsing->symbol = 0;
		if (line[ft_strlen(line) - 1] == '\n')
			add_word_to_list(asm_parsing, create_word(asm_parsing,
									ft_strdup("next line"), NEXT_LINE));
		else
			add_word_to_list(asm_parsing, create_word(asm_parsing,
									ft_strdup("end line"), END_LINE));
		ft_strdel(&line);
	}
	if (ret == -1)
		ft_arg_error("You can't open directory");
}
