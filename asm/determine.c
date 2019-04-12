/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 15:37:45 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/01 15:37:46 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_word	*determine_commands(t_asm *asm_parsing)
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
