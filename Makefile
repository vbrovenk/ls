# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/11 14:06:49 by vbrovenk          #+#    #+#              #
#    Updated: 2019/05/11 14:06:50 by vbrovenk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

INCLUDE_DIR = includes/
INCLUDE = includes/ls.h

LIBFT_INC = ./libft/includes/
LIBFT = ./libft/libft.a

SRC =	main.c \
		sorting.c

OBJ = $(addprefix obj/, $(SRC:.c=.o))

CFLAGS = -Wall -Wextra -Werror

OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITE=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

all: $(NAME)

$(NAME): libft/libft.a obj $(OBJ)
	@gcc $(OBJ) $(LIBFT) -o $(NAME)
	@echo "$(PURPLEBOLD)ft_ls $(PURPLE)is ready$(OFF)"

obj/%.o: src/%.c $(INCLUDE)
	@gcc -c $< -o $@ -I $(LIBFT_INC) -I $(INCLUDE_DIR)
	@echo "$(PURPLELIGHT)Compiling $(WHITE)$< $(PURPLELIGHT)done$(OFF)"

libft/libft.a:
	@make -C libft/

obj:
	@mkdir obj

clean:
	@make clean -C libft/
	@rm -rf obj

fclean: clean
	@make fclean -C libft/
	@rm -rf $(NAME)

re: fclean all
