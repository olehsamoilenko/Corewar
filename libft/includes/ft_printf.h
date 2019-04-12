/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 16:05:44 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/06/26 13:11:13 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

typedef	struct	s_box
{
	int			minus;
	int			plus;
	int			space;
	int			zero;
	int			hash;
	int			width;
	int			precision;
	char		*length;
	char		type;
	int			start;
	int			sum_zeroes;
	char		*prefix;
}				t_box;

int				g_fd;

int				ft_printf(const char *format, ...);
int				print_dec(va_list arg, t_box *info);
int				print_unsigned(va_list arg, t_box *info);
int				print_hex(va_list arg, t_box *info);
int				print_octal(va_list arg, t_box *info);
int				print_percent(t_box *info);
int				print_string(va_list arg, t_box *info);
int				print_char(va_list arg, t_box *info);
int				print_uni_char(va_list arg, t_box *info);
int				sizeof_sym(int c);
int				print_uni_string(va_list arg, t_box *info);
char			*output(t_box *info, char *number);
int				is_length(char c);
int				is_flag(char c);
int				is_correct_type(char t);
void			reset_box(t_box *info);
void			put_flag(t_box *info, char c);
int				sizeof_sym(int c);
int				print(va_list arg, t_box *info);
int				output_str(t_box *info, char *string);

#endif
