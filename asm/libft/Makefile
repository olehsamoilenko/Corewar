# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 11:01:01 by vbrovenk          #+#    #+#              #
#    Updated: 2018/03/24 11:01:03 by vbrovenk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

INC_DIR = ./includes/

SRC =	ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memccpy.c \
		ft_memmove.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strlen.c \
		ft_strdup.c \
		ft_strcpy.c \
		ft_strncpy.c \
		ft_strcat.c \
		ft_strncat.c \
		ft_strlcat.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strstr.c \
		ft_strnstr.c \
		ft_strcmp.c \
		ft_strncmp.c \
		ft_atoi.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_memalloc.c \
		ft_memdel.c \
		ft_strnew.c \
		ft_strdel.c \
		ft_strclr.c \
		ft_striter.c \
		ft_striteri.c \
		ft_strmap.c \
		ft_strmapi.c \
		ft_strequ.c \
		ft_strnequ.c \
		ft_strsub.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_strsplit.c \
		ft_itoa.c \
		ft_putchar.c \
		ft_putstr.c \
		ft_putendl.c \
		ft_putnbr.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_lstnew.c \
		ft_lstdelone.c \
		ft_lstdel.c \
		ft_lstadd.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_countdigits.c \
		ft_isspace.c \
		ft_list_size.c \
		ft_isprime.c \
		ft_lstadd_back.c \
		get_next_line.c

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


all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@echo "$(YELLOW)libft.a $(YELLOWBOLD)-(OK)- $(OFF)" 

%.o: %.c
	@gcc $(CFLAGS) -c $< -o $@ -I $(INC_DIR)
	@echo "$(GREEN)Compiling $(GREENBOLD)$<$(OFF) $(GREEN)done.$(OFF)"

clean:
	@rm -rf $(OBJ)
	@echo "$(PURPLE)Objects $(REDBOLD)$(OBJ)$(OFF) $(PURPLE)have been destroyed.$(OFF)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(PURPLE)Binary $(REDBOLD)$(NAME)$(OFF) $(PURPLE)have been destroyed.$(OFF)"

re: fclean all
