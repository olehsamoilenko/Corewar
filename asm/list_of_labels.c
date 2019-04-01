/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_labels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 17:50:56 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/25 17:50:58 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_label		*create_label(char *name, int refer)
{
	t_label *new_label;

	new_label = (t_label*)malloc(sizeof(t_label));
	new_label->name = name;
	new_label->refer = refer;
	new_label->next = NULL;
	return (new_label);
}

void		add_label_to_list(t_asm *asm_parsing, t_label *new_label)
{
	t_label *temp;

	if (asm_parsing->labels == NULL)
		asm_parsing->labels = new_label;
	else
	{
		temp = asm_parsing->labels;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_label;
	}
}
