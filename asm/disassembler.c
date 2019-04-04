/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disassembler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 10:27:11 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/04 10:27:13 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static void	read_code(t_asm *asm_parsing, int size_code)
{
	unsigned char	*all_code;
	int				instruction;
	int				ret;
	unsigned char	codage;

	all_code = (unsigned char *)malloc(sizeof(char) * size_code);
	ret = read(asm_parsing->fd, all_code, size_code);
	while (asm_parsing->position != size_code)
	{
		instruction = parse_bytecode(asm_parsing, all_code, 1);
		if (instruction > 16)
			error_dis("Unknown instruction");
		if (g_op_tab[instruction - 1].codage_octal == 1)
		{
			codage = parse_bytecode(asm_parsing, all_code, 1);
			ft_printf("%s ", g_op_tab[instruction - 1].name);
			get_args_codage(asm_parsing, all_code, instruction, codage);
		}
		else
		{
			ft_printf("%s ", g_op_tab[instruction - 1].name);
			get_args(asm_parsing, all_code, instruction, codage);
		}
	}
	free(all_code);
}

static void	print_result(t_asm *asm_parsing)
{
	g_fd = 1;
	ft_printf("Writing to file %s\n", asm_parsing->filename);
	ft_strdel(&asm_parsing->filename);
}

static void	file_rename(t_asm *asm_parsing, const char *filename)
{
	char *file_to_write;

	filename = ft_strsub(filename, 0, ft_strlen(filename) - 4);
	file_to_write = ft_strjoin(filename, ".s");
	asm_parsing->fd_dis = open(file_to_write,
								O_CREAT | O_WRONLY | O_TRUNC, 0644);
	g_fd = asm_parsing->fd_dis;
	ft_strdel(&asm_parsing->filename);
	asm_parsing->filename = file_to_write;
	free((char *)filename);
}

void		disassembler(const char *filename)
{
	t_asm	*asm_parsing;
	int		ret;
	int		size_code;
	int		fd;

	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_arg_error("Can't open this file");
	asm_parsing = init_asm(fd, filename);
	file_rename(asm_parsing, filename);
	ret = help_dis(asm_parsing, 4);
	if (ret != COREWAR_EXEC_MAGIC)
		error_dis("MAGIC is wrong");
	read_name(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	size_code = help_dis(asm_parsing, 4);
	asm_parsing->size_code = size_code;
	read_comment(asm_parsing);
	ret = help_dis(asm_parsing, 4);
	if (ret != 0)
		error_dis("NULL is wrong");
	read_code(asm_parsing, size_code);
	print_result(asm_parsing);
	free(asm_parsing);
}
