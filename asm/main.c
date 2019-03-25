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

	// added
	asm_parsing->byte_code = NULL;
	asm_parsing->position = 0;
	return (asm_parsing);
}

// added 
void	write_int_to_byte(t_asm *asm_parsing, int nbr, int size)
{
	int i;
	int count_bits;
	char *temp;
	int position;


	count_bits = 0;
	i = 0;
	position = asm_parsing->position;

	asm_parsing->byte_code = realloc(asm_parsing->byte_code, asm_parsing->position + size);
	while (size)
	{
		asm_parsing->byte_code[position + size - 1] = (nbr >> count_bits) & 0xFF;
		asm_parsing->position++;
		count_bits += 8;
		size--;
	}
}


int		additional_code(int number)
{
	printf("NUMBER = %d\n", number);
	int i;
	int result;
	char temp;

	i = 0;
	result = 0;
	while (i < 32)
	{
		temp = !((number >> i) & 1);
		result = result | (temp << i);
		i++;
	}
	result += 1;
	printf("RES = %d\n", result);
	return (result);
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
				if ((asm_parsing->name_champ == NULL) && (current->word_type == DOUBLES))
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
				if ((asm_parsing->comment == NULL) && (current->word_type == DOUBLES))
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

t_instruction *init_instruction_args(void)
{
	t_instruction *instruction_args;
	int i;

	instruction_args = (t_instruction *)malloc(sizeof(t_instruction));
	i = -1;
	while (++i < 3)
		instruction_args->args[i] = NULL;
	return (instruction_args);
}

t_word	*process_instruction(t_asm *asm_parsing, t_word *current)
{
	int		instruction;
	int		count_args;
	int		sum;

	t_instruction *instruction_args;


	// for codage
	unsigned char var_for_codage = 0;
	char shift_left = 6;

	count_args = 0;
	instruction = find_instruction(current);
	if (instruction == -1)
		error_word2(current, "Incorrect instruction");
	current = current->next;
	// printf("Instruction = %d | count_args = %d\n", instruction, g_op_tab[instruction].count_args);

	instruction_args = init_instruction_args();

	while (current->word_type != NEXT_LINE && current->word_type != END_LINE)
	{
		if (current->word_type == DIRECT_ARG && count_args < g_op_tab[instruction].count_args)
		{
			char two = 2;
			// printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 2 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			var_for_codage = var_for_codage | (two << shift_left);
			shift_left -= 2;
			// printf("var_for_codage =  %d\n", var_for_codage);		
			instruction_args->args[count_args] = current;	

			count_args++;
		}
		else if (current->word_type == INDIRECT_ARG && count_args < g_op_tab[instruction].count_args)
		{
			char three = 3;
			// printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 3 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			var_for_codage = var_for_codage | (three << shift_left);
			shift_left -= 2;
			// printf("var_for_codage =  %d\n", var_for_codage);	
			instruction_args->args[count_args] = current;			

			count_args++;			
		}
		else if (current->word_type == REGISTER && count_args < g_op_tab[instruction].count_args)
		{
			char one = 1;
			// printf("args = %d\n", g_op_tab[instruction].type_args[count_args]);
			sum = 1 | g_op_tab[instruction].type_args[count_args];
			if (sum != g_op_tab[instruction].type_args[count_args])
				error_word2(current, "Incorrect argument");
			var_for_codage = var_for_codage | (one << shift_left);
			shift_left -= 2;
			// printf("var_for_codage =  %d\n", var_for_codage);	
			instruction_args->args[count_args] = current;			
						
			count_args++;

		}
		current = current->next;
		// printf("type = %d\n", current->word_type);
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

	// write opcode of instruction
	write_int_to_byte(asm_parsing, instruction + 1, 1);

	// write codage of instruction
	if (g_op_tab[instruction].codage_octal == 1)
		write_int_to_byte(asm_parsing, var_for_codage, 1);

	// writing args of instruction
	int i = 0;
	int number;
	while (i < 3)
	{
		if (instruction_args->args[i] != NULL)
		{
			if (instruction_args->args[i]->word_type == INDIRECT_ARG)
			{
				number = ft_atoi(instruction_args->args[i]->name);
				write_int_to_byte(asm_parsing, number, 2);
			}
			else if (instruction_args->args[i]->word_type == DIRECT_ARG)
			{
				number = ft_atoi(&instruction_args->args[i]->name[1]);
				if (g_op_tab[instruction].label_size == 1)
					write_int_to_byte(asm_parsing, number, 2);
				else
				{
					// printf("NUMBER = %d\n", number);
					write_int_to_byte(asm_parsing, number, 4);					
				}
			}
			else if (instruction_args->args[i]->word_type == REGISTER)
			{
				number = ft_atoi(&instruction_args->args[i]->name[1]);
				write_int_to_byte(asm_parsing, number, 1);
			}
		}
		i++;
	}

	// printf("var_for_codage =  %d\n", var_for_codage);
	if (count_args != g_op_tab[instruction].count_args)
		error_word2(current, "Wrong number of arguments");
	free(instruction_args);
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
				// printf("label is OK\n");
				current = process_instruction(asm_parsing, current);
			}
			else
				error_word2(current, "Error with labels");
		}
		else if (current->word_type == INSTRUCTION)
		{
			current = process_instruction(asm_parsing, current);
		}
		else
		{
			error_word2(current, "INCORRECT");
		}

	}
}

void	write_data_to_all(char *all, int start, int data, int size)
{
	int i;
	int count_bits;

	count_bits = 0;
	i = 0;
	while (size)
	{
		all[start + size - 1] = (data >> count_bits) & 0xFF;
		count_bits += 8;
		size--;
	}
}

void	write_to_file(t_asm *asm_parsing)
{
	int position_all = 0;

	int len = asm_parsing->position;
	char *all_code = ft_strnew(4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4 + len);
	ft_bzero(all_code, 4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4 + len);
	// added
	{
		write_data_to_all(all_code, position_all, COREWAR_EXEC_MAGIC, 4);
		position_all += 4;

		char *name_without = ft_strsub(asm_parsing->name_champ, 1, ft_strlen(asm_parsing->name_champ) - 2);
		ft_memcpy(&all_code[position_all], name_without, ft_strlen(name_without));
		position_all += (PROG_NAME_LENGTH + 4);

		// write size of champion
		write_data_to_all(all_code, position_all, asm_parsing->position, 4);
		position_all += 4;

		char *comment_without = ft_strsub(asm_parsing->comment, 1, ft_strlen(asm_parsing->comment) - 2);
		ft_memcpy(&all_code[position_all], comment_without, ft_strlen(comment_without));
		position_all += (COMMENT_LENGTH + 4);


		// error with code of champion
		ft_memcpy(&all_code[position_all], asm_parsing->byte_code, asm_parsing->position);

		int new_fd = open("our_test.cor", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		// printf("LENGTH = %d\n", (4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4 + len));
		write(new_fd, all_code, (4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4 + len));

		ft_strdel(&name_without);
		ft_strdel(&comment_without);
		ft_strdel(&all_code);
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

	// added
	{
		write_to_file(asm_parsing);
	}

	free_list(asm_parsing);
	ft_strdel(&asm_parsing->byte_code);
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
 