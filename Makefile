#------------------ Config -----------------#
NAME    := minishell
NAME_DEBUG := minishell_debug
SRCS    := \
src/main.c \
src/gc/gc.c \
src/lexer/lexer.c \
src/lexer/lexer_content.c \
src/lexer/lexer_utils.c \
src/lexer/lexer_handler.c \
src/lexer/lexer_syntax.c \
src/ast/ast.c \
src/ast/ast_redir.c \
src/parser/parser.c \
src/parser/parser_utils.c \
src/parser/parser_redir.c \
src/parser/parser_redir_helpers.c \
src/parser/parser_redir_process.c \
src/parser/parser_expansion.c \
src/executor/executor.c \
src/builtins/builtins.c \
src/builtins/echo.c \
src/builtins/cd.c \
src/builtins/pwd.c \
src/builtins/env.c \
src/builtins/export.c \
src/builtins/unset.c \
src/builtins/exit.c \
src/executor/executor_utils.c \
src/executor/executor_expansion.c \
src/executor/executor_expansion_utils.c \
src/executor/executor_cmd_setup.c \
src/executor/executor_path.c \
src/executor/executor_redir.c \
src/executor/executor_builtins.c \
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
src/utils/str.c \
src/signals/signal_handler.c \
src/signals/signal_state.c \

DEBUG_SRCS := \
src/debug/main_debug.c \
src/debug/ast_print.c \
src/debug/token_print.c \
$(filter-out src/main.c, $(SRCS))

#---------------- Variables ----------------#
CC      := cc
CFLAGS  := -Wextra -Wall -Werror -Wunreachable-code -Ofast

LIB_DIR := ./lib
LIBFT   := $(LIB_DIR)/Libft

HEADERS := -I ./include  -I $(LIBFT)/include
LIBS    := $(LIBFT)/libft.a -lreadline

OBJ_DIR := obj
DEBUG_OBJ_DIR := obj_debug
OBJS    := $(SRCS:src/%.c=$(OBJ_DIR)/%.o)
DEBUG_OBJS := $(DEBUG_SRCS:src/%.c=$(DEBUG_OBJ_DIR)/%.o)

#----------------- Targets ----------------#
all: libft $(NAME)

debug: libft $(NAME_DEBUG)

libft:
	@$(MAKE) --no-print-directory -C $(LIBFT)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(DEBUG_OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling debug: $(notdir $<)\n"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) $(LIBS)

$(NAME_DEBUG): $(DEBUG_OBJS)
	@$(CC) $(CFLAGS) $(DEBUG_OBJS) $(HEADERS) -o $(NAME_DEBUG) $(LIBS)

clean:
	@rm -rf $(OBJ_DIR) $(DEBUG_OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME) $(NAME_DEBUG)
	@$(MAKE) --no-print-directory -C $(LIBFT) fclean

re: fclean all

.PHONY: all debug clean fclean re libft
