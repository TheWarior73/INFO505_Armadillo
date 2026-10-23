CC = gcc

CFLAGS = -Wall -Wextra -Werror # Warning all, extra warnings, treats warnings as errors
NAME = armadillo.game

CORE_DIR = ../libs/core
CORE_LIB = $(CORE_DIR)/libElement.a # Path to the static library

SRC = main.c
OBJ = $(SRC:.c=.o) planet.o tests/test_element.o tests/test_planet.o # Sufix substitution, replaces each file extention in $(SRC) from .c to .o 

# Build the program
all: $(NAME)

$(CORE_LIB):
	$(MAKE) -C $(CORE_DIR) 


$(NAME): $(OBJ) $(CORE_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(CORE_LIB) -o $(NAME)

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
