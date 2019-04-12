/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:48:26 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/04 12:48:28 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static int		size_arg(int argument, int instruction)
{
	if (argument == REG_CODE)
		return (1);
	else if (argument == DIR_CODE)
	{
		if (g_op_tab[instruction].label_size == 1)
			return (2);
		else
			return (4);
	}
	else if (argument == IND_CODE)
		return (2);
	return (-1);
}

static void		get_reg_ind(int *args, int i, int instruction, int arg_content)
{
	char *tmp;

	if (args[i] == REG_CODE)
	{
		tmp = ft_itoa(arg_content);
		ft_printf("r%s", tmp);
		if (i + 1 != g_op_tab[instruction - 1].count_args)
			ft_printf(", ");
		else
			ft_printf("\n");
	}
	else if (args[i] == IND_CODE)
	{
		tmp = ft_itoa((short)arg_content);
		ft_printf("%s", tmp);
		if (i + 1 != g_op_tab[instruction - 1].count_args)
			ft_printf(", ");
		else
			ft_printf("\n");
	}
	ft_strdel(&tmp);
}

static void		get_dir(t_asm *asm_parsing, int instruction, int arg_content,
																	int i)
{
	char *tmp;

	if (g_op_tab[instruction - 1].label_size == 1)
		tmp = ft_itoa((short)arg_content);
	else
		tmp = ft_itoa(arg_content);
	ft_printf("%%%s", tmp);
	if (i + 1 != g_op_tab[instruction - 1].count_args)
		ft_printf(", ");
	else
		ft_printf("\n");
	ft_strdel(&tmp);
}

void			get_args_codage(t_asm *asm_parsing, unsigned char *all_code,
									int instruction, unsigned char codage)
{
	int args[3];
	int i;
	int arg_size;
	int arg_content;

	args[0] = (codage >> 6) & 3;
	args[1] = (codage >> 4) & 3;
	args[2] = (codage >> 2) & 3;
	i = 0;
	while (i < 3)
	{
		arg_size = size_arg(args[i], instruction);
		if (arg_size == -1)
			break ;
		arg_content = parse_bytecode(asm_parsing, all_code, arg_size);
		if (args[i] == REG_CODE)
			get_reg_ind(args, i, instruction, arg_content);
		else if (args[i] == DIR_CODE)
			get_dir(asm_parsing, instruction, arg_content, i);
		else if (args[i] == IND_CODE)
			get_reg_ind(args, i, instruction, arg_content);
		if (arg_size == -1)
			break ;
		i++;
	}
}

void			get_args(t_asm *asm_parsing, unsigned char *all_code,
									int instruction, unsigned char codage)
{
	int		arg;
	int		arg_size;
	int		arg_content;
	char	*tmp;

	if (g_op_tab[instruction - 1].label_size == 0)
		arg_size = 4;
	else
		arg_size = 2;
	arg_content = parse_bytecode(asm_parsing, all_code, arg_size);
	ft_printf("%%");
	if (g_op_tab[instruction - 1].label_size == 0)
		tmp = ft_itoa(arg_content);
	else
		tmp = ft_itoa((short)arg_content);
	ft_printf("%s\n", tmp);
	ft_strdel(&tmp);
}
