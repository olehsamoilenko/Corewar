/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:50:05 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 14:50:06 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static	void	free_list(t_asm *asm_parsing)
{
	t_word	*current;
	t_word	*temp;
	t_label	*current_label;
	t_label	*temp_label;

	current = asm_parsing->words;
	current_label = asm_parsing->labels;
	while (current != NULL)
	{
		temp = current->next;
		ft_strdel(&current->name);
		free(current);
		current = temp;
	}
	while (current_label != NULL)
	{
		temp_label = current_label->next;
		free(current_label);
		current_label = temp_label;
	}
}

void			free_all(t_asm *asm_parsing)
{
	free_list(asm_parsing);
	ft_strdel(&asm_parsing->filename);
	ft_strdel(&asm_parsing->byte_code);
	free(asm_parsing);
}

int				check_for_disasm(char const *argv[])
{
	if (ft_strequ(argv[1], "-d"))
		return (1);
	return (0);
}
