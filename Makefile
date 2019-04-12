# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osamoile <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/01 15:53:40 by osamoile          #+#    #+#              #
#    Updated: 2019/04/05 11:38:02 by sbasnaka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all: asm/asm vm/corewar

asm/asm:
	@make -C asm

vm/corewar:
	@make -C vm

clean:
	@make -C asm clean
	@make -C vm clean
	
fclean: clean
	@make -C asm fclean
	@make -C vm fclean

re: fclean all
