#------------------ Config -----------------#
NAME    := minishell
SRCS    := \
src/main.c \
src/gc/gc.c \
src/lexer/lexer.c \
src/lexer/lexer_utils.c \
src/lexer/lexer_handler.c \
src/lexer/lexer_quotes.c \
src/ast/ast.c \
src/ast/ast_redir.c \
src/ast/ast_print.c \
src/parser/parser.c \
src/parser/parser_utils.c \
src/parser/parser_redir.c \
src/parser/parser_redir_helpers.c \
src/parser/parser_redir_process.c \
src/parser/parser_expansion.c \
src/executor/executor.c \
src/executor/executor_utils.c \
src/executor/executor_expansion.c \
src/executor/executor_expansion_utils.c \
src/executor/executor_path.c \
src/executor/executor_redir.c \
src/executor/executor_multi_redir.c \
src/executor/executor_pipes.c \
src/heredoc/heredoc_collection.c \
src/heredoc/heredoc_main.c \
src/heredoc/heredoc_expansion_vars.c \
src/heredoc/heredoc_expansion_utils.c \
src/heredoc/heredoc_expansion_loop.c \
src/heredoc/heredoc_redirection.c \
src/env/env_storage.c \
src/env/env_init.c \
src/env/env_utils.c \
src/env/env_print.c \
src/error/error.c \

#---------------- Variables ----------------#
CC      := cc
CFLAGS  := -Wextra -Wall -Werror -Wunreachable-code -Ofast

LIB_DIR := ./lib
LIBFT   := $(LIB_DIR)/Libft

HEADERS := -I ./include  -I $(LIBFT)/include
LIBS    := $(LIBFT)/libft.a -lreadline

OBJ_DIR := obj
OBJS    := $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

#----------------- Targets ----------------#
all: libft $(NAME)

libft:
	@$(MAKE) --no-print-directory -C $(LIBFT)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) $(LIBS)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT) fclean

re: clean all

.PHONY: all clean fclean re libft
