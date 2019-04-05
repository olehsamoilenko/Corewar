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

NAME =			vm
HEADER =		./includes/vm.h
INC =			-I ./includes \
				-I ./libft/includes
LIB =			-lncurses -lft -L ./libft
FLAGS =			#-Wall -Wextra -Werror
LIST =			main \
				op \
				params \
				champion \
				init \
				carriage \
				verbose \
				curses \
				tmp
OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))

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

obj/%.o: src/%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@ $(INC)
	@echo "$(PURPLELIGHT)Compiling $(OFF)\033[1m$< $(OFF)$(PURPLELIGHT)done$(OFF)"

$(NAME): obj $(OBJ)
	@make -C libft
	@gcc $(OBJ) -o $(NAME) $(LIB)
	@echo "$(PURPLEBOLD)$(NAME)$(OFF)$(PURPLE) is ready$(OFF)"

obj:
	@mkdir obj

clean:
	@make -C libft clean
	@rm -rf obj
	
fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all
