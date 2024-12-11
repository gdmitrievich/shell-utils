# VARIABLES
TARGET = s21_grep
COMMON_DIR = ../common

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

RM = rm -rf

# TOOLS
CLANG_FORMAT_OPTS = -i
CPPCHECK_OPTS = --enable=all --template=gcc --language=c

# SOURCES
SRCS = $(wildcard *.c)
SRCS += $(COMMON_DIR)/common.c
OBJS = $(SRCS:%.c=%.o)

# TARGETS
all: tools
all: $(TARGET)
.PHONY: all

rebuild: full_clean all
.PHONY: rebuild

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(COMMON_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
.PHONY: clean

full_clean: clean
	$(RM) $(TARGET)
.PHONY: full_clean

tools:
	@clang-format -i $(SRCS) *.h
	@cppcheck $(CPPCHECK_OPTS) $(SRCS)
.PHONY: tools