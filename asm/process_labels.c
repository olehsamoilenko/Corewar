/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_labels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 17:42:08 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 17:42:10 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_word	*work_with_args(t_word *current, t_instruction *instruction_args,
															t_codage *codage)
{
	if (current->word_type == DIRECT_ARG && codage->count_args <
									g_op_tab[codage->instruction].count_args)
	{
		instruction_args->args[codage->count_args] = current;
		codage->count_args++;
	}
	else if (current->word_type == INDIRECT_ARG && codage->count_args <
									g_op_tab[codage->instruction].count_args)
	{
		instruction_args->args[codage->count_args] = current;
		codage->count_args++;
	}
	else if (current->word_type == REGISTER && codage->count_args <
									g_op_tab[codage->instruction].count_args)
	{
		instruction_args->args[codage->count_args] = current;
		codage->count_args++;
	}
	current = current->next;
	return (current);
}

void	count_bytes(t_asm *asm_parsing, t_codage *codage,
							t_instruction *instruction_args, t_word *current)
{
	int i;

	asm_parsing->pos_labels += 1;
	if (g_op_tab[codage->instruction].codage_octal == 1)
		asm_parsing->pos_labels += 1;
	i = -1;
	while (++i < 3)
		if (instruction_args->args[i] != NULL)
		{
			if (instruction_args->args[i]->word_type == INDIRECT_ARG)
				asm_parsing->pos_labels += 2;
			else if (instruction_args->args[i]->word_type == DIRECT_ARG)
			{
				if (g_op_tab[codage->instruction].label_size == 1)
					asm_parsing->pos_labels += 2;
				else
					asm_parsing->pos_labels += 4;
			}
			else if (instruction_args->args[i]->word_type == REGISTER)
				asm_parsing->pos_labels += 1;
		}
	if (codage->count_args != g_op_tab[codage->instruction].count_args)
		error_word2(current, "Wrong number of arguments");
	free(instruction_args);
	free(codage);
}

t_word	*process_label(t_asm *asm_parsing, t_word *current)
{
	t_codage		*codage;
	t_instruction	*instruction_args;

	codage = init_codage();
	codage->instruction = find_instruction(current);
	if (codage->instruction == -1)
		error_word2(current, "Incorrect instruction");
	current = current->next;
	instruction_args = init_instruction_args();
	while (current->word_type != NEXT_LINE && current->word_type != END_LINE)
	{
		current = work_with_args(current, instruction_args, codage);
		if (codage->count_args == g_op_tab[codage->instruction].count_args)
		{
			if (current->word_type != NEXT_LINE &&
										current->word_type != END_LINE)
				error_word2(current, "Invalid syntax after instruction");
		}
		else if (current->word_type == SEPARATOR)
			current = current->next;
		else
			error_word2(current, "Separator is missed");
	}
	count_bytes(asm_parsing, codage, instruction_args, current);
	return (current);
}
