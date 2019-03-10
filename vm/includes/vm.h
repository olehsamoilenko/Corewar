/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 11:39:25 by osamoile          #+#    #+#             */
/*   Updated: 2018/08/02 11:39:26 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "libft.h"
# include "op.h"
# include <stdio.h> // trash


// reversed bytes ??? [ 17 00 00 00 ] is 23
union magic_header
{
	int				hex;
	unsigned char	bytes[4];
};


typedef struct	s_war
{
	unsigned char	map[MEM_SIZE];

}				t_war;

typedef struct	s_champion
{
	// char			name[PROG_NAME_LENGTH];
	// char			comment[COMMENT_LENGTH];
	// int				size;
	header_t		*header;
}				t_champion;



#endif
