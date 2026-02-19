#------------------ Config -----------------#
NAME    := minishell
SRCS    := \
src/core/main.c \
src/core/memory.c \
src/tokenizer/tokenizer.c \
src/tokenizer/content.c \
src/tokenizer/concat.c \
src/tokenizer/utils.c \
src/tokenizer/handler.c \
src/tokenizer/syntax.c \
src/ast/ast.c \
src/ast/ast_redir.c \
src/ast/ast_redir_utils.c \
src/parser/parser.c \
src/parser/parser_utils.c \
src/parser/parser_redir.c \
src/parser/parser_redir_entry.c \
src/parser/parser_redir_helpers.c \
src/parser/parser_redir_count.c \
src/parser/parser_token_utils.c \
src/parser/parser_redir_process.c \
src/expansion/variables.c \
src/expansion/expansion.c \
src/expansion/utils.c \
src/expansion/heredoc_vars.c \
src/expansion/heredoc_utils.c \
src/expansion/heredoc_loop.c \
src/executor/executor.c \
src/executor/executor_utils.c \
src/executor/executor_helpers.c \
src/executor/executor_cmd_setup.c \
src/executor/executor_path.c \
src/executor/executor_builtins.c \
src/executor/executor_pipes.c \
src/io/heredoc_collection.c \
src/io/heredoc.c \
src/io/redirection.c \
src/io/redirection_ordered.c \
src/io/basic_redir.c \
src/io/multi_redir.c \
src/builtins/builtins.c \
src/builtins/echo.c \
src/builtins/cd.c \
src/builtins/pwd.c \
src/builtins/env.c \
src/builtins/export.c \
src/builtins/unset.c \
src/builtins/exit.c \
src/environment/env_storage.c \
src/environment/env_init.c \
src/environment/env_utils.c \
src/environment/env_print.c \
src/error/error.c \
src/utils/str.c \
src/signals/signal_handler.c \
src/signals/signal_state.c \

#---------------- Variables ----------------#
CC      := cc
CFLAGS  := -Wextra -Wall -Werror -Wunreachable-code -Ofast -Wunused
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

re: fclean all

val: all
	@valgrind -q --suppressions=suppression_file \
				--leak-check=full \
				--show-leak-kinds=all \
				--track-origins=yes \
				--track-fds=yes \
				./${NAME}

.PHONY: all clean fclean re libft
