CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11
NAME = program

CORE_DIR = ../libs/core
CORE_LIB = $(CORE_DIR)/libelement.a

SRC = main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(CORE_LIB):
	$(MAKE) -C $(CORE_DIR)

$(NAME): $(OBJ) $(CORE_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(CORE_LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(CORE_DIR)/Element -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(CORE_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(CORE_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
