/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	adv(t_war *war, t_op *op, int instr_len, t_carriage *car, t_instr_params *params)
{
	if (war->flag_verbose)
	{
		ft_printf("ADV %d (%#06x -> %#06x) %02x ",
			instr_len,
			car->position,
			car->position + instr_len,
			op->code,
			params->codage);
		if (op->codage) // why index ?? t_op *op !
			ft_printf("%02x ", params->codage);
		int j = 0;
		while (++j < 4)
		{
			int k = params->sizes[j];
			while(--k >= 0)
				ft_printf("%02x ", params->params[j].bytes[k]);
			
		}
		ft_printf("\n");
	}
}

void	dump(t_war *war)
{
	int i = -1;

	// introducing

	while (++i < MEM_SIZE)
	{
		if (i % 64 == 0)
		{
			if (i == 0)
				ft_printf("0x0000 : ");
			else
				ft_printf("\n%#06x : ", i);
		}
			
		ft_printf("%02x ", war->map[i]->value);
	}
	ft_printf("\n");
}