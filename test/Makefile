#------------------ Config -----------------#
TARGET = test_runner
SOURCES =              \
	../src/gc/gc.c \
	../src/lexer/lexer.c \
	../src/lexer/lexer_utils.c \
	../src/lexer/lexer_handler.c \
	../src/lexer/lexer_quotes.c \
	../src/ast/ast.c \
	../src/ast/ast_redir.c \
	../src/debug/ast_print.c \
	../src/debug/token_print.c \
	../src/parser/parser.c \
	../src/parser/parser_utils.c \
	../src/parser/parser_redir.c \
	../src/parser/parser_redir_helpers.c \
	../src/parser/parser_redir_process.c \
	../src/parser/parser_expansion.c \
	../src/executor/executor.c \
	../src/builtins/builtins.c \
	../src/builtins/echo.c \
	../src/builtins/cd.c \
	../src/builtins/pwd.c \
	../src/builtins/env.c \
	../src/builtins/export.c \
	../src/builtins/unset.c \
	../src/builtins/exit.c \
	../src/executor/executor_utils.c \
	../src/executor/executor_expansion.c \
	../src/executor/executor_expansion_utils.c \
	../src/executor/executor_path.c \
	../src/executor/executor_redir.c \
	../src/executor/executor_builtins.c \
	../src/executor/executor_multi_redir.c \
	../src/executor/executor_pipes.c \
	../src/heredoc/heredoc_collection.c \
	../src/heredoc/heredoc_main.c \
	../src/heredoc/heredoc_expansion_vars.c \
	../src/heredoc/heredoc_expansion_utils.c \
	../src/heredoc/heredoc_expansion_loop.c \
	../src/heredoc/heredoc_redirection.c \
	../src/env/env_storage.c \
	../src/env/env_init.c \
	../src/env/env_utils.c \
	../src/env/env_print.c \
	../src/error/error.c \
	../src/signals/signal_handler.c \
	../src/signals/signal_state.c \
	test_gc.c      \
	test_lexer.c   \
	test_ast.c     \
	test_parser.c  \
	test_executor.c \
	test_builtins.c \
	test_heredoc.c \
	test_env.c \
	test_signals.c \
	test_error.c \

#---------------- Variables ----------------#
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
CRITERION_LIB_DIR = ./criterion/lib
CRITERION_INCLUDE_DIR = ./criterion/include
LINKFLAGS = -L$(CRITERION_LIB_DIR) -lcriterion ../lib/Libft/libft.a -lreadline

#----------------- Targets ----------------#
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -I$(CRITERION_INCLUDE_DIR) -I../include -I../lib/Libft/include/ -I../src/debug $(SOURCES) $(LINKFLAGS) -o $(TARGET)

test: $(TARGET)
	LD_LIBRARY_PATH=$(CRITERION_LIB_DIR) ./$(TARGET) --verbose

clean:
	rm -f $(TARGET)

.PHONY: all clean test
