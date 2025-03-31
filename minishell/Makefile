# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcurty-g <rcurty-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/31 11:44:46 by rcurty-g          #+#    #+#              #
#    Updated: 2025/03/31 12:07:21 by rcurty-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
LIBFLAGS= -lreadline
RM		= rm -rf

INCLUDE	= -Iinclude/
SRC_DIR	= src/
OBJ_DIR	= obj/
LIBFT_DIR = libft/
LIBFT	= libft/libft.a

READLINE_SUPP = readline.supp

SRC_FILES = main.c \
			init_minishell.c constructors.c parse_func.c \
			parse_utils.c parse_utils2.c env_utils.c syntax_check.c \
			clean_token.c clean_token_utils.c expansions.c \
			expansions2.c parse_redirs.c \
			executor.c executor2.c pipe_exec.c redirs_exec.c \
			heredoc.c heredoc_utils.c executor_utils.c redirs_exec2.c \
			signals.c \
			builtins.c echo.c pwd.c export.c export2.c export3.c \
			unset.c env.c exit.c exit2.c utils.c cd.c cd2.c \
			free_functions.c

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

all: $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJ)
	@make -C $(LIBFT_DIR) -s
	@$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(NAME) $(LIBFLAGS) $(LIBFT)
	@echo "Executable $(NAME) created successfully."
	@echo "$(NAME) compiled successfully."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "Object directory created successfully."

clean:
	@make clean -C $(LIBFT_DIR) -s
	@$(RM) $(OBJ_DIR)
	@echo "Clean completed successfully."

fclean: clean
	@make fclean -C $(LIBFT_DIR) -s
	@$(RM) $(NAME)
	@echo "Full clean completed successfully."

re: fclean all
	@echo "Rebuild completed successfully."

valgrind: re
	valgrind -s -q --suppressions=$(READLINE_SUPP) \
	--tool=memcheck --leak-check=full \
	--show-leak-kinds=all --track-origins=yes \
	--show-below-main=no ./$(NAME)
	@echo "Valgrind execution completed."

.PHONY: all clean fclean re le
