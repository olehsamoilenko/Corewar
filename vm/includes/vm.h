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
	int					hex;
	unsigned char		bytes[4];
};

typedef struct	s_champion
{
	char				*file;
	header_t			*header;
}				t_champion;

typedef struct	s_map_cell
{
	unsigned char		value;
	int					color;
}				t_mem_cell;

typedef struct	s_carriage
{
	int					position;
	int					color;
	struct s_carriage	*next;
}				t_carriage;

typedef struct	s_war
{
	t_mem_cell			*map[MEM_SIZE];
	t_champion			*champs[5];
	t_carriage			*carriages;

}				t_war;





#endif
