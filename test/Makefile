#------------------ Config -----------------#
TARGET = test_runner
SOURCES =              \
	../src/gc/gc.c \
	../src/lexer/lexer.c \
	../src/lexer/lexer_utils.c \
	../src/lexer/lexer_handler.c \
	../src/ast/ast.c \
	../src/ast/ast_redir.c \
	../src/ast/ast_print.c \
	../src/parser/parser.c \
	../src/parser/parser_utils.c \
	../src/parser/parser_redir.c \
	../src/parser/parser_redir_helpers.c \
	../src/executor/executor.c \
	../src/executor/executor_path.c \
	../src/executor/executor_redir.c \
	../src/executor/executor_multi_redir.c \
	../src/executor/executor_pipes.c \
	test_gc.c      \
	test_lexer.c   \
	test_ast.c     \
	test_parser.c  \
	test_executor.c \

#---------------- Variables ----------------#
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
CRITERION_LIB_DIR = ./criterion/lib
CRITERION_INCLUDE_DIR = ./criterion/include
LINKFLAGS = -L$(CRITERION_LIB_DIR) -lcriterion ../lib/Libft/libft.a

#----------------- Targets ----------------#
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -I$(CRITERION_INCLUDE_DIR) -I../include -I../lib/Libft/include/ $(SOURCES) $(LINKFLAGS) -o $(TARGET)

test: $(TARGET)
	LD_LIBRARY_PATH=$(CRITERION_LIB_DIR) ./$(TARGET) --verbose

clean:
	rm -f $(TARGET)

.PHONY: all clean test
