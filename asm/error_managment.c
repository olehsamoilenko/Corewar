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
	// system("leaks asm");
	exit(1);
}

void	error_dis(char *message)
{
	ft_putendl_fd(message, 2);
	exit(1);
}

void	ft_error(t_asm *asm_parsing, char *message)
{
	ft_printf("asm_parsing->row = %d\n", asm_parsing->row);
	ft_printf("asm_parsing->symbol = %d\n", asm_parsing->symbol);
	ft_printf("%s\n", message);
	// system("leaks asm");
	exit(1);
}

void	error_word(t_asm *asm_parsing, char *message)
{
	ft_printf("Not valid word on row %d \"%s\"\n", asm_parsing->row, message);
	// system("leaks asm");
	exit(1);
}

void	error_word2(t_word *word, char *message)
{
	ft_printf("ERROR: \"%s\" on row %d\n", message, word->row);
	// system("leaks asm");
	exit(1);
}
