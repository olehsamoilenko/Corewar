/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 15:31:49 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/24 15:31:50 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	read_magic_header(int fd)
{
	int i;
	t_converter header;
	unsigned char tmp;

	header.integer = COREWAR_EXEC_MAGIC;
	i = -1;
	while (++i < 4)
	{
		read(fd, &tmp, 1);
		if (tmp != header.bytes[3 - i])
			return (0);
	}
	return (1);
}

void	read_name(int fd, char *name)
{
	int i;
	int res;

	i = -1;
	res = 0;
	while (++i < PROG_NAME_LENGTH)
		res += read(fd, &name[i], 1);
	if (res != PROG_NAME_LENGTH)
		error(ERR_SMALL_FILE);
}

int		read_null(int fd)
{
	int i;
	int res;
	unsigned int tmp;

	i = -1;
	res = read(fd, &tmp, 4);
	if (res != 4 || tmp != 0)
		return (0);
	return (1);
}

int		read_exec_code_size(int fd)
{
	t_converter size;
	int i = -1;
	int res;

	res = 0;
	i = -1;
	while (++i < 4)
		res += read(fd, &size.bytes[3 - i], 1);
	// ft_printf("%x\n", size.integer);
	return (size.integer);
}

void	read_comment(int fd, char *comment)
{
	int i = -1;
	int res;

	res = 0;
	while (++i < COMMENT_LENGTH)
		res += read(fd, &comment[i], 1);
	if (res != COMMENT_LENGTH)
		error(ERR_SMALL_FILE);
}