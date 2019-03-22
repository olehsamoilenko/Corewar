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

void	error_word2(t_word *word, char *message)
{
	printf("ERROR: \"%s\" on row %d\n", message, word->row);
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
		printf("current->row = %d\n", current->row);
		// printf("current->symbol = %d\n", current->symbol);		
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

t_word		*determine_commands(t_asm *asm_parsing)
{
	t_word *current;

	current = asm_parsing->words;
	while (asm_parsing->name_champ == NULL || asm_parsing->comment == NULL)
	{
		if (current->word_type == COMMAND)
		{
			if (ft_strequ(current->name, ".name"))
			{
				current = current->next;
				if ((asm_parsing->name_champ == NULL) && (current->word_type == DOUBLES) && 
													ft_strlen(current->name) < PROG_NAME_LENGTH)
				{
					asm_parsing->name_champ = current->name;
					current = current->next;
				}
				else if (asm_parsing->name_champ != NULL)
					error_word2(current, "Duplicate of name");
				else
					error_word2(current, "Name is missed");
			}
			else if (ft_strequ(current->name, ".comment"))
			{

				current = current->next;
				if ((asm_parsing->comment == NULL) && (current->word_type == DOUBLES) &&
													ft_strlen(current->name) < COMMENT_LENGTH)
				{
					asm_parsing->comment = current->name;
					current = current->next;
				}
				else if (asm_parsing->comment != NULL)
					error_word2(current, "Duplicate of comment");
				else
					error_word2(current, "Comment is missed");
			}
		}
		if (current->word_type != NEXT_LINE)
			error_word2(current, "Name or comment is missed");
		current = current->next;
	}
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

t_word	*process_instruction(t_word *current)
{
	int		instruction;
	int		count_args;
	int		sum;

	count_args = 0;
	instruction = find_instruction(current);
	if (instruction == -1)
		error_word2(current, "Incorrect instruction");
	current = current->next;
	printf("Instruction = %d | count_args = %d\n", instruction, g_op_tab[instruction].count_args);	
	while (current->word_type != NEXT_LINE && current->word_type != END_LINE)
	{
		if (current->word_type == DIRECT_ARG && count_args < g_op_tab[instruction].count_args)
		{
			printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 2 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			count_args++;
		}
		else if (current->word_type == INDIRECT_ARG && count_args < g_op_tab[instruction].count_args)
		{
			printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 3 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			count_args++;
		}
		else if (current->word_type == REGISTER && count_args < g_op_tab[instruction].count_args)
		{
			printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 1 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			count_args++;
		}
		current = current->next;
		printf("type = %d\n", current->word_type);
		if (count_args == g_op_tab[instruction].count_args)
		{
			// current = current->next;
			if (current->word_type != NEXT_LINE && current->word_type != END_LINE)
				error_word2(current, "Wrong instruction");		
		}
		else if (current->word_type == SEPARATOR)
			current = current->next;
		else
			error_word2(current, "Separator is missed");
	}
	if (count_args != g_op_tab[instruction].count_args)
		error_word2(current, "Wrong number of arguments");
	return (current);
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
			if (current->word_type == INSTRUCTION)
			{
				current = process_instruction(current);
				printf("label is OK\n");
			}
			else
				error_word2(current, "Error with labels");
		}
		else if (current->word_type == INSTRUCTION)
		{
			current = process_instruction(current);
		}
		else
		{
			error_word2(current, "INCORRECT");
		}

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
	asm_parsing = init_asm(fd);
	// print_asm_structure(asm_parsing);
	while ((ret = get_next_line(fd, &line)) > 0 && line != NULL)
	{
		asm_parsing->row++;
		parse_line(asm_parsing, line);
		asm_parsing->symbol = 0;
		if (line[ft_strlen(line) - 1] == '\n')
			add_word_to_list(asm_parsing, create_word(asm_parsing, ft_strdup("next line"), NEXT_LINE));	
		else					
			add_word_to_list(asm_parsing, create_word(asm_parsing, ft_strdup("end line"), END_LINE));	
		ft_strdel(&line);
	}
	if (ret == -1)
		ft_arg_error("You can't open directory");
	print_list(asm_parsing);
	print_asm_structure(asm_parsing);

	current = determine_commands(asm_parsing);
	determine_instructions(asm_parsing, current);
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
 