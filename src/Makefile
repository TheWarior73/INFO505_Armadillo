CC = gcc

CFLAGS = -Wall -Wextra -Werror # Warning all, extra warnings, treats warnings as errors
NAME = armadillo.game

CORE_DIR = ../libs/core
CORE_LIBS = $(CORE_DIR)/libGame.a $(CORE_DIR)/libPlanet.a $(CORE_DIR)/libElement.a
CORE_INCLUDES = -I$(CORE_DIR)/Element -I$(CORE_DIR)/Planet -I$(CORE_DIR)/game

SRC = main.c
OBJ = $(SRC:.c=.o) planet.o tests/test_element.o tests/test_planet.o # Sufix substitution, replaces each file extention in $(SRC) from .c to .o 

# Build the program
all: $(NAME)

$(CORE_LIBS): | core-build

core-build:
	$(MAKE) -C $(CORE_DIR)


$(NAME): $(OBJ) $(CORE_LIBS)
	$(CC) $(CFLAGS) $(OBJ) $(CORE_LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CORE_INCLUDES) -c $< -o $@

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

.PHONY: all clean fclean re core-build