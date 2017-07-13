# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/14 15:29:40 by tle-meur          #+#    #+#              #
#    Updated: 2016/05/25 14:25:23 by tle-meur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	gcc

FLAGS	=	-Wall -Wextra -Werror

SRCS	=	main.c builtins.c get_infos.c utilities.c

OBJS	=	$(SRCS:.c=.o)

all		:	$(NAME)

%.o		:	%.c
			@$(CC) $(FLAGS) -o $@ -c $< -I libft/

$(NAME)	:	$(OBJS)
			@make -C libft/
			@$(CC) $(FLAGS) -o $@ $^ -L libft/ -lft
			@echo "$(NAME) created"
clean	:
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

fclean	:	clean
			@make -C libft/ fclean
			@rm -f $(NAME)
			@echo "$(NAME) deleted"

re		:	fclean $(NAME)

simple	:	re
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

.PHONY	:	all clean fclean re simple
