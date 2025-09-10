#------------------ Config -----------------#
NAME    := minishell
SRCS    := \
src/main.c \
src/gc/gc.c \
src/lexer/lexer.c \
src/lexer/lexer_utils.c \
src/lexer/lexer_handler.c \
src/ast/ast.c \
src/ast/ast_print.c \
src/parser/parser.c \
src/parser/parser_utils.c \
src/executor/executor.c \
src/executor/executor_utils.c \
src/executor/executor_pipes.c \

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
