/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/15 13:36:39 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/04/15 13:36:41 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	save_after_new(t_gnl *gnl, char *end_str)
{
	char *dup_end;

	gnl->length = ft_strlen(end_str);
	end_str += 1;
	dup_end = ft_strdup(end_str);
	ft_bzero(gnl->string, ft_strlen(gnl->string));
	ft_strcpy(gnl->string, dup_end);
	ft_strdel(&dup_end);
}

void	help_func(t_gnl *gnl, char **line, char *end_str)
{
	char *temp;
	char *last_line;

	if (end_str != NULL)
	{
		temp = ft_strsub(gnl->string, 0,
		ft_strlen(gnl->string) - ft_strlen(end_str) + 1);
		save_after_new(gnl, end_str);
	}
	else
	{
		temp = ft_strdup(gnl->string);
		gnl->length = 0;
	}
	last_line = *line;
	if (*line != NULL)
	{
		*line = ft_strjoin(last_line, temp);
		free(temp);
		free(last_line);
	}
	else
		*line = temp;
}

int		get_next_line(const int fd, char **line)
{
	static	t_gnl	gnl[4000];
	char			*end_str;

	if (read(fd, 0, 0) < 0 || line == NULL)
		return (-1);
	*line = 0;
	end_str = 0;
	while (end_str == 0)
	{
		if (gnl[fd].length == 0)
		{
			if ((gnl[fd].length = read(fd, gnl[fd].string, BUFF_SIZE)) < 1)
			{
				if (*line != NULL)
					return (1);
				else
					return (0);
			}
		}
		gnl[fd].string[gnl[fd].length] = '\0';
		end_str = ft_strchr(gnl[fd].string, '\n');
		help_func(&gnl[fd], line, end_str);
	}
	return (1);
}
