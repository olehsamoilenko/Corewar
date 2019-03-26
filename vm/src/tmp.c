/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

// ADDITIONAL FILE

void	curriage_info(t_carriage *car, t_war *war)
{
	if (war->flag_verbose)
	{
		ft_printf("INFO P %d pos: %d, op: ", car->number ,car->position);
		
		if (car->op)
			ft_printf("%s", car->op->name);
		else
			ft_printf("empty");
		ft_printf(", current cooldown: %d\n", car->cooldown);

	}
}

void	print_champion_hex(unsigned char *map)
{
	int i = 0;
	while (i < MEM_SIZE)
	{
		ft_printf("%02x%02x ", map[i], map[i + 1]);
		i += 2;
		if (i % 16 == 0)
			ft_printf("\n");
	}
}

void	print_champions(t_champion *champs[])
{
	
	int i = -1;
	while (++i < 4)
	{
		// ft_printf("\t%i: \n", i + 1);
		if (champs[i] == NULL)
		{
			ft_printf("Empty.\n");
			// return ;
		}
		else
		{
			ft_printf("n = %d, NAME: %s\n", champs[i]->number, champs[i]->header->prog_name);
			// ft_printf("COMMENT: %s\n", champs[i]->header->comment);
			// ft_printf("SIZE: %d\n", champs[i]->header->prog_size);
		}
	}
}

void	reg_info(int *reg, t_war *war)
{
	if (war->flag_verbose)
	{
		int i = -1;
		while (++i <= REG_NUMBER)
			ft_printf("%d ", reg[i]);
		ft_printf("\n");

	}
}

char	*define_type(int type)
{
	if (type == REG_CODE)
		return("T_REG");
	else if (type == DIR_CODE)
		return("T_DIR");
	else if (type == IND_CODE)
		return ("T_IND");
	else
		return ("---");
}

void show_args(t_instr_params *params, t_war *war)
{
	int i = 0;
	while (++i <= params->amount && war->flag_dump == -1)
	{
		ft_printf("ARG %d: ", i);
		ft_printf("%10ld ", params->params[i].integer);
		ft_printf("%#010x ", params->params[i].integer);
		int j = -1;
		while (++j < 4)
			ft_printf("%02x ", params->params[i].bytes[j]);
		ft_printf("%s\n", define_type(params->types[i]));

	}
	// ft_printf("ARGS:\n\t%ld %#06x (%s)\n\t%ld %#06x (%s)\n\t%ld %#06x (%s)\n",
	// 	params->params[1], params->params[1], define_type(params->types[1]),
	// 	params->params[2], params->params[2], define_type(params->types[2]),
	// 	params->params[3], params->params[3], define_type(params->types[3]));
}