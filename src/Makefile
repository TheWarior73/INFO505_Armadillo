CC = gcc

CFLAGS = -Wall -Wextra -Werror # Warning all, extra warnings, treats warnings comme errors
NAME = armadillo

# ---
# Core Library of the project
# ---
CORE_DIR = ../libs/core
CORE_LIB = $(CORE_DIR)/libElement.a # Path to the static library

# ---
# Source files
# ---
SRC = main.c
OBJ = $(SRC:.c=.o) # Sufix substitution, replaces each file extention in $(SRC) from .c to .o 

# ---

# Build the program
all: $(NAME)

# Build the core library
#
# Specifically :
# $(MAKE) tells make to run again
# -C $(CORE_DIR) tells make to go in the core library directory before running
$(CORE_LIB):
	$(MAKE) -C $(CORE_DIR) 


# Build armadillo 
# 
# Depends on: Objetc files & core library
#
# Links the static lib and the objects together then outputs the runnable file under $(NAME) -> (armadillo)
$(NAME): $(OBJ) $(CORE_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(CORE_LIB) -o $(NAME)

# Builds a .c file
# -I$(CORE_DIR)/Element adds a directory for make to search header files in.
# -c $< : filename.c ($< uses the input files)
# -o $@ : fileName.o ($@ refers to the output name)
%.o: %.c
	$(CC) $(CFLAGS) -I$(CORE_DIR)/Element -c $< -o $@

# Cleans the .o files in both this directory and calls `make clean` in the library directory
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(CORE_DIR) clean

# Cleans all the build files, even the executable files
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(CORE_DIR) fclean

# cleans everything, then recompiles the project
re: fclean all

# Tells make that these names are command names, if a file is called the same, it won't override the command usage.
# Somewhat useless for our usecase as of now.
.PHONY: all clean fclean re
