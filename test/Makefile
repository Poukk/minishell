#------------------ Config -----------------#
TARGET = test_runner
SOURCES =              \
	../src/gc/gc.c \
	../src/lexer/lexer.c \
	../src/lexer/lexer_utils.c \
	../src/lexer/lexer_handler.c \
	test_gc.c      \
	test_lexer.c   \

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
