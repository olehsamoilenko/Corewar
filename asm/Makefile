# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/09 14:05:55 by vbrovenk          #+#    #+#              #
#    Updated: 2019/03/09 14:05:56 by vbrovenk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = asm
INCLUDE = asm.h
LIBFT_INC = ./libft/includes
LIBFT = libft/libft.a
SRC =	main.c \
		list_of_words.c \
		list_of_labels.c \
		parsing_lines.c \
		op.c \
		check_all.c \
		error_managment.c \
		convert_and_write.c \
		free_all.c \
		process_instruction.c \
		process_labels.c \
		cut_functions.c \
		process_args.c \
		write_args.c \
		help_functions.c \
		determine.c \
		disassembler.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror


OFF=\033[0m

RED=\033[31m
REDBOLD=\033[1;31m
GREEN=\033[32m
GREENBOLD=\033[1;32m
YELLOW=\033[33m
YELLOWBOLD=\033[1;33m
PURPLE=\033[35m
PURPLEBOLD=\033[1;35m

PURPLELIGHT=\033[38;2;102;102;255m

all: $(NAME)

$(NAME): lib $(OBJ)
	@gcc $(OBJ) $(LIBFT) -o $(NAME)
	@echo "$(PURPLEBOLD)ASM$(OFF)$(PURPLE) is ready$(OFF)"

lib:
	@make -C libft/

%.o: %.c $(INCLUDE)
	@gcc -c $< -o $@ -I $(LIBFT_INC)
	@echo "$(PURPLELIGHT)Compiling $(OFF)\033[1m$< $(OFF)$(PURPLELIGHT)done$(OFF)"

clean:
	@make clean -C libft/
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C libft/
	@rm -rf $(NAME)

re: fclean all