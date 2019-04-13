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

t_word	*process_args(t_word *current, t_codage *codage,
									t_instruction *instruction_args)
{
	if (current->word_type == DIRECT_ARG &&
			codage->count_args < g_op_tab[codage->instruction].count_args)
		process_direct_arg(codage, instruction_args, current);
	else if (current->word_type == INDIRECT_ARG &&
			codage->count_args < g_op_tab[codage->instruction].count_args)
		process_indirect_arg(codage, instruction_args, current);
	else if (current->word_type == REGISTER &&
			codage->count_args < g_op_tab[codage->instruction].count_args)
		process_register_arg(codage, instruction_args, current);
	if (current->word_type == INSTRUCTION)
		error_word2(current, "Not valid argument");
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
