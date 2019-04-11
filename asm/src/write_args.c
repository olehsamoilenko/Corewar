/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 15:21:20 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/01 15:21:21 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	write_ind_arg(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct)
{
	t_label	*label;
	int		number;

	if (ft_strchr(instruction_args->args[i]->name, ':'))
	{
		label = find_label(asm_parsing, instruction_args->args[i]);
		if (label != NULL)
			write_int_to_byte(asm_parsing,
						label->refer - position_of_instruct, 2);
		else
			error_word2(instruction_args->args[i], "Unknown label");
	}
	else
	{
		number = ft_atoi(instruction_args->args[i]->name);
		write_int_to_byte(asm_parsing, number, 2);
	}
}

void	if_exist_label(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct)
{
	t_label	*label;

	label = find_label(asm_parsing, instruction_args->args[i]);
	if (label != NULL)
	{
		if (g_op_tab[instruction_args->opcode].label_size == 1)
			write_int_to_byte(asm_parsing,
					label->refer - position_of_instruct, 2);
		else
			write_int_to_byte(asm_parsing,
					label->refer - position_of_instruct, 4);
	}
	else
		error_word2(instruction_args->args[i], "Unknown label");
}

void	write_dir_arg(t_asm *asm_parsing, int i,
					t_instruction *instruction_args, int position_of_instruct)
{
	int		number;

	if (ft_strchr(instruction_args->args[i]->name, ':'))
		if_exist_label(asm_parsing, i, instruction_args, position_of_instruct);
	else
	{
		number = ft_atoi(&instruction_args->args[i]->name[1]);
		if (g_op_tab[instruction_args->opcode].label_size == 1)
			write_int_to_byte(asm_parsing, number, 2);
		else
			write_int_to_byte(asm_parsing, number, 4);
	}
}

void	define_arg_type(t_asm *asm_parsing, t_instruction *instruction_args,
											int i, int position_of_instruct)
{
	int		number;

	if (instruction_args->args[i]->word_type == INDIRECT_ARG)
		write_ind_arg(asm_parsing, i, instruction_args,
									position_of_instruct);
	else if (instruction_args->args[i]->word_type == DIRECT_ARG)
		write_dir_arg(asm_parsing, i, instruction_args,
									position_of_instruct);
	else if (instruction_args->args[i]->word_type == REGISTER)
	{
		number = ft_atoi(&instruction_args->args[i]->name[1]);
		write_int_to_byte(asm_parsing, number, 1);
	}
}

void	write_args(t_asm *asm_parsing, t_codage *codage,
									t_instruction *instruction_args)
{
	int	i;
	int	position_of_instruct;

	position_of_instruct = asm_parsing->position;
	write_int_to_byte(asm_parsing, codage->instruction + 1, 1);
	if (g_op_tab[codage->instruction].codage_octal == 1)
		write_int_to_byte(asm_parsing, codage->var_for_codage, 1);
	i = -1;
	while (++i < 3)
		if (instruction_args->args[i] != NULL)
			define_arg_type(asm_parsing, instruction_args,
											i, position_of_instruct);
}
