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

# NAME =			asm
# HEADER =		./includes/asm.h
# INC =			-I ./includes \
# 				-I ../libft/includes
# LIB =			-lft -L ../libft
# FLAGS =			-Wall -Wextra -Werror
# LIST =			main \
# 				list_of_words \
# 				list_of_labels \
# 				parsing_lines \
# 				op \
# 				check_all \
# 				error_managment \
# 				convert_and_write \
# 				free_all \
# 				process_instruction \
# 				process_labels \
# 				cut_functions \
# 				process_args \
# 				write_args \
# 				help_functions \
# 				determine \
# 				disassembler \
# 				get_args \
# 				read_and_parse
# OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))

# OFF=\033[0m
# PURPLE=\033[0;35m
# PURPLEBOLD=\033[1;35m
# WHITE=\033[1;37m
# PURPLELIGHT=\033[38;2;102;102;255m

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
