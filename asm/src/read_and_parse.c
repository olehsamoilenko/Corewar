/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_and_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:48:35 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/04 12:48:36 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

unsigned int	from_bytes_to_int(unsigned char *buf, int size)
{
	int res;
	int shift;
	int i;

	i = size;
	i--;
	shift = 0;
	res = 0;
	while (size != 0)
	{
		res = res | (buf[i] << shift);
		i--;
		shift += 8;
		size--;
	}
	return (res);
}

int				help_dis(t_asm *asm_parsing, int size)
{
	unsigned char	*buf;
	int				size_ret;
	int				res;

	buf = (unsigned char *)malloc(sizeof(unsigned char) * size);
	size_ret = read(asm_parsing->fd, buf, size);
	if (size_ret == 0)
		error_dis("Error with reading from file");
	res = from_bytes_to_int(buf, size);
	free(buf);
	return (res);
}

void			read_name(t_asm *asm_parsing)
{
	char	*str;
	int		size;

	str = (char *)malloc(sizeof(char ) * 128);
	size = read(asm_parsing->fd, str, 128);
	if (size == 0)
		error_dis("Error with reading from file");
	ft_printf(".name \"%s\"\n", str);
	ft_strdel(&str);
}

void			read_comment(t_asm *asm_parsing)
{
	char	*str;
	int		size;

	str = (char *)malloc(sizeof(char ) * 2048);
	size = read(asm_parsing->fd, str, 2048);
	if (size == 0)
		error_dis("Error with reading from file");
	ft_printf(".comment \"%s\"\n\n", str);
	ft_strdel(&str);
}

int				parse_bytecode(t_asm *asm_parsing, unsigned char *all_code,
																	int size)
{
	int res;
	int shift;
	int i;

	i = asm_parsing->position + size;
	i--;
	if (asm_parsing->size_code < i)
		error_dis("ERROR");
	shift = 0;
	res = 0;
	while (size != 0)
	{
		res = res | (all_code[i] << shift);
		shift += 8;
		i--;
		asm_parsing->position++;
		size--;
	}
	return (res);
}
