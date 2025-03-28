# Nome do executável
NAME	= minishell

# Compilador a ser usado
CC		= cc

# Flags de compilação: avisos e debug
CFLAGS		= -Wall -Wextra -Werror -g

# Flags para linkar a biblioteca readline
LIBFLAGS	= -lreadline

# Comando para remover arquivos
RM			= rm -rf

# Diretórios de inclusão e código
INCLUDE		= -Iinclude/
SRC_DIR		= src/
OBJ_DIR		= obj/
LIBFT_DIR	= libft/

# Caminho para a libft compilada
LIBFT		= libft/libft.a

# Arquivo de log para leaks com valgrind
LEAKS_LOG		= ./leaks.log

# Arquivo de supressão de leaks da readline
READLINE_SUPP	= readline.supp

# Flags para execução do valgrind
VALGRINDFLAGS	= -s -q --suppressions=$(READLINE_SUPP) \
				  --tool=memcheck --leak-check=full \
				  --show-leak-kinds=all --track-origins=yes \
				  --show-below-main=no

# Lista dos arquivos fonte (.c) do projeto
SRC_FILES	= main.c \
			  init_minishell.c constructors.c parse_func.c \
			  parse_utils.c parse_utils2.c env_utils.c syntax_check.c \
			  clean_token.c clean_token_utils.c expansions.c \
			  expansions2.c parse_redirs.c \
			  executor.c executor2.c pipe_exec.c redirs_exec.c \
			  heredoc.c heredoc_utils.c executor_utils.c redirs_exec2.c \
			  signals.c \
			  builtins.c echo.c pwd.c export.c export2.c export3.c \
			  unset.c env.c exit.c exit2.c utils.c cd.c cd2.c \
			  free_functions.c \

# Gera caminho completo dos fontes com base no diretório
SRC			= $(addprefix $(SRC_DIR), $(SRC_FILES))

# Gera os arquivos objeto (.o) correspondentes
OBJ			= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# ============================================================================ #
#	COLORS                                                                     #
# ============================================================================ #

# Códigos de cor para exibição no terminal
CLR_RMV	= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
R_GREEN	= \033[0;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

# ============================================================================ #
#	RULES                                                                      #
# ============================================================================ #

# Compilação padrão
all: $(NAME)

# Regra para compilar os arquivos objeto
$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Regra para criar o executável
$(NAME): $(OBJ_DIR) $(OBJ)
	@ make -C $(LIBFT_DIR) -s
	@ echo "Compilating ${YELLOW}$(NAME) ${CLR_RMV}..."
	@ $(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(NAME) $(LIBFLAGS) $(LIBFT)

# Cria o diretório de objetos se não existir
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Limpa os arquivos objeto
clean:
	@ make clean -C $(LIBFT_DIR) -s
	@ $(RM) $(OBJ_DIR)
	@ echo "$(RED)Deleting $(CLR_RMV)object files"

# Limpa os arquivos objeto e binário
fclean: clean
	@ make fclean -C $(LIBFT_DIR) -s
	@ $(RM) $(NAME)
	@ echo "$(RED)Deleting $(CLR_RMV)binary"

# Reconstrói o projeto
re: fclean all

# Executa com valgrind
le: re
	valgrind $(VALGRINDFLAGS) ./$(NAME)

# Conteúdo do arquivo de supressão do valgrind para readline
define SUP_BODY
{
   name
   Memcheck:Leak
   fun:*alloc
   ...
   obj:*/libreadline.so.*
   ...
}
{
    leak readline
    Memcheck:Leak
    ...
    fun:readline
}
{
    leak add_history
    Memcheck:Leak
    ...
    fun:add_history
}
endef

# Gera o arquivo de supressão
sup:
	$(file > readline.supp,$(SUP_BODY))

# Define alvos como não arquivos
.PHONY: all clean fclean re le sup
