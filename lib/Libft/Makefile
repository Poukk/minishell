#------------------ Config -----------------#
NAME = libft.a
SRCS = \
ctype/ft_isalnum.c                            \
ctype/ft_isalpha.c                            \
ctype/ft_isascii.c                            \
ctype/ft_isdigit.c                            \
ctype/ft_isprint.c                            \
ctype/ft_isxdigit.c                           \
ctype/ft_isspace.c                            \
ctype/ft_tolower.c                            \
ctype/ft_toupper.c                            \
io/ft_putchar_fd.c                            \
io/ft_putendl_fd.c                            \
io/ft_putnbr_fd.c                             \
io/ft_putstr_fd.c                             \
linked_list/ft_lstadd_back_bonus.c            \
linked_list/ft_lstadd_front_bonus.c           \
linked_list/ft_lstclear_bonus.c               \
linked_list/ft_lstdelone_bonus.c              \
linked_list/ft_lstiter_bonus.c                \
linked_list/ft_lstlast_bonus.c                \
linked_list/ft_lstmap_bonus.c                 \
linked_list/ft_lstnew_bonus.c                 \
linked_list/ft_lstsize_bonus.c                \
stdlib/ft_atoi.c                              \
stdlib/ft_calloc.c                            \
stdlib/ft_strtol.c                            \
stdlib/ft_itoa.c                              \
string/ft_bzero.c                             \
string/ft_memchr.c                            \
string/ft_memcmp.c                            \
string/ft_memcpy.c                            \
string/ft_memmove.c                           \
string/ft_memset.c                            \
string/ft_split.c                             \
string/ft_strchr.c                            \
string/ft_strdup.c                            \
string/ft_striteri.c                          \
string/ft_strjoin.c                           \
string/ft_strlcat.c                           \
string/ft_strlcpy.c                           \
string/ft_strlen.c                            \
string/ft_strmapi.c                           \
string/ft_strncmp.c                           \
string/ft_strnstr.c                           \
string/ft_strrchr.c                           \
string/ft_strtrim.c                           \
string/ft_substr.c                            \
string/count_words.c                          \
ft_printf/printf_number.c                     \
ft_printf/printf_hex.c                        \
ft_printf/printf_char.c                       \
ft_printf/printf.c                            \
get_next_line/get_next_line.c                 \
get_next_line/utils.c                         \

#---------------- Variables ----------------#
CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
INCLUDES = -I./include

GREEN = $$(tput setaf 2)
RESET = $$(tput sgr0)

#----------------- Targets ----------------#
all: $(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@echo "$(GREEN)Created libft object directory$(RESET)"

$(NAME): $(OBJDIR) $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	@echo "$(GREEN)Cleaned object files$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Cleaned library$(RESET)"

re: fclean all

.PHONY: all clean fclean re
