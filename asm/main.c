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

// DELETE THIS
// void	print_asm_structure(t_asm *asm_parsing)
// {
// 	printf("===========  ASM_PARSING  ==========\n");
// 	printf("asm_parsing->name_champ = %s\n", asm_parsing->name_champ);
// 	printf("asm_parsing->comment = %s\n", asm_parsing->comment);
// 	printf("asm_parsing->row = %d\n", asm_parsing->row);
// 	printf("asm_parsing->symbol = %d\n", asm_parsing->symbol);
// }

// void	print_list(t_asm *asm_parsing)
// {
// 	t_word	*current;

// 	current = asm_parsing->words;
// 	while (current != NULL)
// 	{
// 		printf("current->name = %s\n", current->name);
// 		printf("current->word_type = %s\n", test[current->word_type - 1]);
// 		printf("current->row = %d\n", current->row);
// 		// printf("current->symbol = %d\n", current->symbol);		
// 		printf("======================================\n");
// 		current = current->next;
// 	}
// }

// void	print_labels(t_asm *asm_parsing)
// {
// 	t_label *current_label;

// 	current_label = asm_parsing->labels;
// 	while (current_label != NULL)
// 	{
// 		printf("label->name = %s | label->refer = %d\n", current_label->name, current_label->refer);
// 		current_label = current_label->next;
// 	}
// }

t_asm	*init_asm(int fd, const char *filename)
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
	return (asm_parsing);
}

void	ignore_comment(t_asm *asm_parsing, char *line)
{
	if (line[asm_parsing->symbol] == COMMENT_CHAR)
		while (line[asm_parsing->symbol])
			asm_parsing->symbol++;
}

char	*take_word(int end, char *line, int start)
{
	char	*temp;

	temp = ft_strsub(line, start, end - start);
	return (temp);
}

t_word	*determine_name(t_asm *asm_parsing, t_word *current)
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

t_word	*determine_comment(t_asm *asm_parsing, t_word *current)
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

t_word		*determine_commands(t_asm *asm_parsing)
{
	t_word *current;

	current = asm_parsing->words;
	while (asm_parsing->name_champ == NULL || asm_parsing->comment == NULL)
	{
		if (current->word_type == COMMAND)
		{
			if (ft_strequ(current->name, ".name"))
				current = determine_name(asm_parsing, current);
			else if (ft_strequ(current->name, ".comment"))
				current = determine_comment(asm_parsing, current);
		}
		if (current->word_type != NEXT_LINE)
			error_word2(current, "Name or comment is missed");
		current = current->next;
	}
	if (ft_strlen(asm_parsing->name_champ) - 2 > PROG_NAME_LENGTH)
		ft_arg_error("Length of the name is bigger than 128");
	if (ft_strlen(asm_parsing->comment) - 2 > COMMENT_LENGTH)
		ft_arg_error("Length of the comment is bigger than 2048");
	return (current);
}

int	find_instruction(t_word *current)
{
	int i;

	i = 0;
	while (i < 17)
	{
		if (ft_strequ(current->name, g_op_tab[i].name))
			return (i);
		i++;
	}
	return (-1);
}

t_instruction	*init_instruction_args(void)
{
	t_instruction	*instruction_args;
	int				i;

	instruction_args = (t_instruction *)malloc(sizeof(t_instruction));
	i = -1;
	while (++i < 3)
		instruction_args->args[i] = NULL;
	return (instruction_args);
}

t_label	*find_label(t_asm *asm_parsing, t_word *current_label)
{
	t_label	*label;
	char	*name;
	char	*name_label;

	label = asm_parsing->labels;
	name = ft_strchr(current_label->name, ':') + 1;
	while (label != NULL)
	{
		name_label = ft_strsub(label->name, 0, ft_strlen(label->name) - 1);
		if (ft_strcmp(name, name_label) == 0)
		{
			ft_strdel(&name_label);
			return (label);
		}
		ft_strdel(&name_label);
		label = label->next;
	}
	return (NULL);
}

void	determine_instructions(t_asm *asm_parsing, t_word *current)
{
	while (current->word_type != END_LINE)
	{
		if (current->word_type == NEXT_LINE)
			current = current->next;
		else if (current->word_type == LABEL)
		{
			current = current->next;
			while (current->word_type == NEXT_LINE)
				current = current->next;
			if (current->word_type == END_LINE)
				break ;
			else if (current->word_type == LABEL)
				continue ;
			if (current->word_type == INSTRUCTION)
				current = process_instruction(asm_parsing, current);
			else
				error_word2(current, "Error with labels");
		}
		else if (current->word_type == INSTRUCTION)
			current = process_instruction(asm_parsing, current);
		else
			error_word2(current, "INCORRECT");
	}
}

t_word	*add_labels(t_asm *asm_parsing, t_word *current, int *ret,
													t_word *current_label)
{
	if (current_label->next->word_type == NEXT_LINE &&
											current->word_type == END_LINE)
	{
		add_label_to_list(asm_parsing, create_label(current_label->name,
												asm_parsing->pos_labels));
		*ret = BREAK;
	}
	else if (current->word_type == LABEL &&
								current_label->next->word_type == NEXT_LINE)
	{
		add_label_to_list(asm_parsing, create_label(current_label->name,
													asm_parsing->pos_labels));
		*ret = CONTINUE;
	}
	else if (current->word_type == INSTRUCTION)
	{
		add_label_to_list(asm_parsing, create_label(current_label->name,
													asm_parsing->pos_labels));
		current = process_label(asm_parsing, current);
	}
	else
		error_word2(current, "Error with labels");
	return (current);
}

void	determine_labels(t_asm *asm_parsing, t_word *current)
{
	t_word	*current_label;
	int		ret;

	ret = 0;
	while (current->word_type != END_LINE)
	{
		if (current->word_type == NEXT_LINE)
			current = current->next;
		else if (current->word_type == LABEL)
		{
			current_label = current;
			current = current->next;
			while (current->word_type == NEXT_LINE)
				current = current->next;
			current = add_labels(asm_parsing, current, &ret, current_label);
			if (ret == BREAK)
				break ;
			else if (ret == CONTINUE)
				continue ;
		}
		else if (current->word_type == INSTRUCTION)
			current = process_label(asm_parsing, current);
		else
			error_word2(current, "INCORRECT");
	}
}

void	interpreter(const char *filename)
{
	int		fd;
	char	*line;
	int		ret;
	t_asm	*asm_parsing;
	t_word	*current;

	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd, filename);
	check_all_lines(asm_parsing, ret, fd, line);
	// print_list(asm_parsing);
	current = determine_commands(asm_parsing);
	determine_labels(asm_parsing, current);
	// print_labels(asm_parsing);
	determine_instructions(asm_parsing, current);
	write_to_file(asm_parsing);
	free_all(asm_parsing);
}

int		main(int argc, char const *argv[])
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
	
	// system("leaks asm");
	return (0);
}
