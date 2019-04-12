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

void	adv(t_war *war, t_op *op, int instr_len, t_carriage *car)
{
	int j;
	if (war->flag_verbose && war->cycle >= war->flag_dump)
	{
		ft_printf("ADV %d (", instr_len);
		
		if (car->position == 0)
			ft_printf("0x0000");
		else
			ft_printf("%#06x", car->position);

		ft_printf(" -> ");
		
		if (car->position + instr_len == 0)
			ft_printf("0x0000");
		else
			ft_printf("%#06x", car->position + instr_len);
		
		ft_printf(") ");
		j = -1;
		while (++j < instr_len)
			ft_printf("%02x ", war->map[(car->position + j) % MEM_SIZE]->value);
		ft_printf("\n");

	}
}

void	verbose_won(t_war *war)
{
	if (!war->flag_visual &&
	(war->flag_dump == -1 || war->flag_dump >= war->cycle))
	{
		ft_printf("Contestant %d, \"%s\", has won !\n",
			war->last_live->number, war->last_live->header->prog_name);
	}
}


void	dump(t_war *war)
{
	int i = -1;

	if (!war->flag_visual && !war->flag_verbose)
	{
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
}