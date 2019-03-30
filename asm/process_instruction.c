/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_instruction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:57:30 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 14:57:32 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	process_direct_arg(t_codage *codage, t_instruction *instruction_args,
																t_word *current)
{
	codage->sum = T_DIR | g_op_tab[codage->instruction].type_args[codage->count_args];
	if (codage->sum != g_op_tab[codage->instruction].type_args[codage->count_args])
		error_word2(current, "Incorrect argument");
	codage->var_for_codage = codage->var_for_codage | (DIR_CODE << codage->shift_left);
	codage->shift_left -= 2;
	instruction_args->args[codage->count_args] = current;
	codage->count_args++;
}

void	process_indirect_arg(t_codage *codage, t_instruction *instruction_args,
																t_word *current)
{
	codage->sum = T_IND | g_op_tab[codage->instruction].type_args[codage->count_args];
	if (codage->sum != g_op_tab[codage->instruction].type_args[codage->count_args])
		error_word2(current, "Incorrect argument");
	codage->var_for_codage = codage->var_for_codage | (IND_CODE << codage->shift_left);
	codage->shift_left -= 2;
	instruction_args->args[codage->count_args] = current;
	codage->count_args++;
}

void	process_register_arg(t_codage *codage, t_instruction *instruction_args,
																t_word *current)
{
	codage->sum = T_REG | g_op_tab[codage->instruction].type_args[codage->count_args];
	if (codage->sum != g_op_tab[codage->instruction].type_args[codage->count_args])
		error_word2(current, "Incorrect argument");
	codage->var_for_codage = codage->var_for_codage | (REG_CODE << codage->shift_left);
	codage->shift_left -= 2;
	instruction_args->args[codage->count_args] = current;			
	codage->count_args++;
}

t_codage	*init_codage(void)
{
	t_codage	*codage;

	codage = (t_codage *)malloc(sizeof(t_codage));
	codage->instruction = 0;
	codage->count_args = 0;
	codage->sum = 0;
	codage->var_for_codage = 0;
	codage->shift_left = 6;
	return (codage);
}

void	write_ind_arg(t_asm *asm_parsing, int i, t_instruction *instruction_args,
														int position_of_instruct)
{
	t_label	*label;
	int		number;

	if (ft_strchr(instruction_args->args[i]->name, ':'))
	{
		label = find_label(asm_parsing, instruction_args->args[i]);
		if (label != NULL)
			write_int_to_byte(asm_parsing, label->refer - position_of_instruct, 2);
		else
			error_word2(instruction_args->args[i], "Unknown label");
	}
	else
	{
		number = ft_atoi(instruction_args->args[i]->name);
		write_int_to_byte(asm_parsing, number, 2);
	}
}

void	write_dir_arg(t_asm *asm_parsing, int i, t_instruction *instruction_args, int position_of_instruct)
{
	t_label	*label;
	int		number;

	if (ft_strchr(instruction_args->args[i]->name, ':'))
	{
		t_label *label = find_label(asm_parsing, instruction_args->args[i]);
		if (label != NULL)
		{
			if (g_op_tab[instruction_args->opcode].label_size == 1)
				write_int_to_byte(asm_parsing, label->refer - position_of_instruct, 2);
			else
				write_int_to_byte(asm_parsing, label->refer - position_of_instruct, 4);							
		}
		else
			error_word2(instruction_args->args[i], "Unknown label");
	}
	else
	{
		number = ft_atoi(&instruction_args->args[i]->name[1]);
		if (g_op_tab[instruction_args->opcode].label_size == 1)
			write_int_to_byte(asm_parsing, number, 2);
		else
			write_int_to_byte(asm_parsing, number, 4);
	}
}

void	write_args(t_asm *asm_parsing, t_codage *codage, t_instruction *instruction_args)
{
	int				i;
	int				number;
	int				position_of_instruct;
	
	position_of_instruct = asm_parsing->position;
	write_int_to_byte(asm_parsing, codage->instruction + 1, 1);
	if (g_op_tab[codage->instruction].codage_octal == 1)
		write_int_to_byte(asm_parsing, codage->var_for_codage, 1);
	i = -1;
	while (++i < 3)
	{
		if (instruction_args->args[i] != NULL)
		{
			if (instruction_args->args[i]->word_type == INDIRECT_ARG)
				write_ind_arg(asm_parsing, i, instruction_args, position_of_instruct);
			else if (instruction_args->args[i]->word_type == DIRECT_ARG)
				write_dir_arg(asm_parsing, i, instruction_args, position_of_instruct);
			else if (instruction_args->args[i]->word_type == REGISTER)
			{
				number = ft_atoi(&instruction_args->args[i]->name[1]);
				write_int_to_byte(asm_parsing, number, 1);
			}
		}
	}
}

t_word	*process_args(t_word *current, t_codage *codage, t_instruction *instruction_args)
{
	if (current->word_type == DIRECT_ARG && codage->count_args < g_op_tab[codage->instruction].count_args)
		process_direct_arg(codage, instruction_args, current);
	else if (current->word_type == INDIRECT_ARG && codage->count_args < g_op_tab[codage->instruction].count_args)
		process_indirect_arg(codage, instruction_args, current);
	else if (current->word_type == REGISTER && codage->count_args < g_op_tab[codage->instruction].count_args)
		process_register_arg(codage, instruction_args, current);
	current = current->next;
	if (codage->count_args == g_op_tab[codage->instruction].count_args)
	{
		if (current->word_type != NEXT_LINE)			
			error_word2(current, "Invalid syntax after instruction");
	}
	else if (current->word_type == SEPARATOR)
		current = current->next;
	else
		error_word2(current, "Separator is missed");
	return (current);
}

t_word	*process_instruction(t_asm *asm_parsing, t_word *current)
{
	t_instruction	*instruction_args;
	t_codage		*codage;
	
	codage = init_codage();
	codage->instruction = find_instruction(current);
	if (codage->instruction == -1)
		error_word2(current, "Incorrect instruction");
	current = current->next;
	instruction_args = init_instruction_args();
	instruction_args->opcode = codage->instruction;
	while (current->word_type != NEXT_LINE && current->word_type != END_LINE)
		current = process_args(current, codage, instruction_args);
	write_args(asm_parsing, codage, instruction_args);
	if (codage->count_args != g_op_tab[codage->instruction].count_args)
		error_word2(current, "Wrong number of arguments");
	free(instruction_args);
	free(codage);
	return (current);
}
