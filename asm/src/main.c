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

t_asm		*init_asm(int fd, const char *filename)
{
	t_asm	*asm_parsing;

	if (!(asm_parsing = (t_asm *)malloc(sizeof(t_asm))))
		ft_arg_error("Can't allocate memory");
	asm_parsing->filename = ft_strsub(filename, 0, ft_strlen(filename) - 2);
	asm_parsing->fd = fd;
	asm_parsing->name_champ = NULL;
	asm_parsing->comment = NULL;
	asm_parsing->row = 0;
	asm_parsing->symbol = 0;
	asm_parsing->words = NULL;
	asm_parsing->labels = NULL;
	asm_parsing->byte_code = NULL;
	asm_parsing->position = 0;
	asm_parsing->pos_labels = 0;
	asm_parsing->disassembler = 0;
	asm_parsing->asm_code = NULL;
	return (asm_parsing);
}

t_word		*determine_name(t_asm *asm_parsing, t_word *current)
{
	current = current->next;
	if ((asm_parsing->name_champ == NULL) && (current->word_type == DOUBLES))
	{
		asm_parsing->name_champ = current->name;
		current = current->next;
	}
	else if (asm_parsing->name_champ != NULL)
		error_word2(current, "Duplicate of name");
	else
		error_word2(current, "Name is missed");
	return (current);
}

t_word		*determine_comment(t_asm *asm_parsing, t_word *current)
{
	current = current->next;
	if ((asm_parsing->comment == NULL) && (current->word_type == DOUBLES))
	{
		asm_parsing->comment = current->name;
		current = current->next;
	}
	else if (asm_parsing->comment != NULL)
		error_word2(current, "Duplicate of comment");
	else
		error_word2(current, "Comment is missed");
	return (current);
}

static void	assembler(const char *filename)
{
	int		fd;
	char	*line;
	int		ret;
	t_asm	*asm_parsing;
	t_word	*current;

	ret = 0;
	line = NULL;
	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd, filename);
	check_all_lines(asm_parsing, ret, fd, line);
	current = determine_commands(asm_parsing);
	determine_labels(asm_parsing, current);
	determine_instructions(asm_parsing, current);
	write_to_file(asm_parsing);
	free_all(asm_parsing);
}

int			main(int argc, char const *argv[])
{
	char *extention;

	if (argc != 2 && argc != 3)
		ft_arg_error("Usage: ./asm [-d] champion.s\n\t-d\tdisassembler");
	if ((check_for_disasm(argv)))
	{
		if (argc != 3)
			ft_arg_error("Missed filename");
		extention = ft_strdup(ft_strrchr(argv[2], '.'));
		if (!ft_strequ(extention, ".cor"))
			ft_arg_error("Extention of the file must be .cor");
		if (ft_strlen(argv[2]) < 5)
			ft_arg_error("Missed filename");
		disassembler(argv[2]);
		// system("leaks asm");
		return (0);
	}
	g_fd = 2;
	extention = ft_strdup(ft_strrchr(argv[1], '.'));
	if (!ft_strequ(extention, ".s"))
		ft_arg_error("Extention of the file must be .s");
	if (ft_strlen(argv[1]) < 3)
		ft_arg_error("Missed filename");
	assembler(argv[1]);
	// system("leaks asm");
	return (0);
}
