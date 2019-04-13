/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_managment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:34:39 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 14:34:42 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	ft_arg_error(char *message)
{
	ft_putendl_fd(message, 2);
	exit(1);
}

void	error_dis(char *message)
{
	ft_putendl_fd(message, 2);
	exit(1);
}

void	ft_error(t_asm *asm_parsing)
{
	ft_printf("Not valid token on row %d in symbol %d\n", asm_parsing->row,
	asm_parsing->symbol);
	exit(1);
}

void	error_word(t_asm *asm_parsing, char *message)
{
	ft_printf("Not valid token on row %d \"%s\"\n", asm_parsing->row, message);
	exit(1);
}

void	error_word2(t_word *word, char *message)
{
	ft_printf("ERROR: \"%s\" on row %d\n", message, word->row);
	exit(1);
}
