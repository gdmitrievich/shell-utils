# VARIABLES
TARGET = s21_cat

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

RM = rm -rf

# TOOLS
CLANG_FORMAT_OPTS = -i
CPPCHECK_OPTS = --verbose --enable=all --template=gcc --error-exitcode=1 --language=c

# SOURCES
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)

# TARGETS
all: $(TARGET)
.PHONY: all

rebuild: full_clean all
.PHONY: rebuild

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
.PHONY: clean

full_clean: clean
	$(RM) $(TARGET)
.PHONY: full_clean

tools:
	clang-format -i $(SRCS)
	cppcheck $(CPPCHECK_OPTS) $(SRCS)
.PHONY: tools