#------------------ Config -----------------#
NAME    := minishell
SRCS    := \
src/main.c \
src/sum.c  \

#---------------- Variables ----------------#
CC      := cc
CFLAGS  := -Wextra -Wall -Werror -Wunreachable-code -Ofast -lreadline

LIB_DIR := ./lib
LIBFT   := $(LIB_DIR)/Libft

HEADERS := -I ./include  -I $(LIBFT)/include
LIBS    := $(LIBFT)/libft.a

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
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT) fclean

re: clean all

.PHONY: all clean fclean re libft
