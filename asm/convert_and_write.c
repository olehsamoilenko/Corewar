/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_and_write.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:41:19 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/03/30 14:41:24 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	write_int_to_byte(t_asm *asm_parsing, int nbr, int size)
{
	int		i;
	int		count_bits;
	char	*temp;
	int		position;

	count_bits = 0;
	i = 0;
	position = asm_parsing->position;
	asm_parsing->byte_code = realloc(asm_parsing->byte_code,
												asm_parsing->position + size);
	while (size)
	{
		asm_parsing->byte_code[position + size - 1] =
													(nbr >> count_bits) & 0xFF;
		asm_parsing->position++;
		count_bits += 8;
		size--;
	}
}

void	write_data_to_all(char *all, int start, int data, int size)
{
	int i;
	int count_bits;

	count_bits = 0;
	i = 0;
	while (size)
	{
		all[start + size - 1] = (data >> count_bits) & 0xFF;
		count_bits += 8;
		size--;
	}
}

int		write_name_comment(t_asm *asm_parsing, char *all_code, int position_all)
{
	char *name_without;
	char *comment_without;

	name_without = ft_strsub(asm_parsing->name_champ, 1,
									ft_strlen(asm_parsing->name_champ) - 2);
	ft_memcpy(&all_code[position_all], name_without,
									ft_strlen(name_without));
	position_all += (PROG_NAME_LENGTH + 4);
	write_data_to_all(all_code, position_all, asm_parsing->position, 4);
	position_all += 4;
	comment_without = ft_strsub(asm_parsing->comment, 1,
									ft_strlen(asm_parsing->comment) - 2);
	ft_memcpy(&all_code[position_all], comment_without,
									ft_strlen(comment_without));
	position_all += (COMMENT_LENGTH + 4);
	ft_strdel(&name_without);
	ft_strdel(&comment_without);
	return (position_all);
}

void	write_to_file(t_asm *asm_parsing)
{
	int		position_all;
	char	*file_to_write;
	char	*all_code;
	int		new_fd;

	all_code = ft_strnew(4 + PROG_NAME_LENGTH + 4 + 4 +
								COMMENT_LENGTH + 4 + asm_parsing->position);
	position_all = 0;
	file_to_write = ft_strjoin(asm_parsing->filename, ".cor");
	ft_bzero(all_code, 4 + PROG_NAME_LENGTH + 4 + 4 +
								COMMENT_LENGTH + 4 + asm_parsing->position);
	write_data_to_all(all_code, position_all, COREWAR_EXEC_MAGIC, 4);
	position_all += 4;
	position_all = write_name_comment(asm_parsing, all_code, position_all);
	ft_memcpy(&all_code[position_all], asm_parsing->byte_code,
													asm_parsing->position);
	new_fd = open(file_to_write, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(new_fd, all_code, (4 + PROG_NAME_LENGTH + 4 + 4 +
								COMMENT_LENGTH + 4 + asm_parsing->position));
	ft_strdel(&all_code);
	printf("Writing output program to %s\n", file_to_write);
	ft_strdel(&file_to_write);
}
