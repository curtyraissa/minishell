NAME	= minishell
CC		= cc
CFLAGS		= -Wall -Wextra -Werror -g
LIBFLAGS	= -lreadline
RM			= rm -rf
INCLUDE		= -Iinclude/
SRC_DIR		= src/
OBJ_DIR		= obj/
LIBFT_DIR	= libft/
LIBFT		= libft/libft.a

# Source files list, TO DO
SRC_FILES	= main.c clean_token.c clean_token_utils.c
SRC			= $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ			= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Main rules
all: $(NAME)
	@echo "All components compiled successfully."

# Object files compilation
$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "Object files created successfully."

# Executable building
$(NAME): $(OBJ_DIR) $(OBJ)
	@ make -C $(LIBFT_DIR) -s
	@ $(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(NAME) $(LIBFLAGS) $(LIBFT)
	@echo "Minishell compiled successfully."

# Object directory creation
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "Object directory created successfully."

# Cleaning object files
clean:
	@ make clean -C $(LIBFT_DIR) -s
	@ $(RM) $(OBJ_DIR)
	@echo "Object files cleaned successfully."

# Full cleanup
fclean: clean
	@ make fclean -C $(LIBFT_DIR) -s
	@ $(RM) $(NAME)
	@echo "All files fully cleaned successfully."

# Recompile everything
re: fclean all
	@echo "Recompilation completed successfully."

# Run with Valgrind
valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./$(NAME)
	@echo "Valgrind check completed successfully."