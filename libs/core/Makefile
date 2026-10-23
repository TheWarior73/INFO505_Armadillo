CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -Werror -std=c11
NAME = libElement.a

OBJ_DIR = build

SRCS = Element/element.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)
$(NAME): $(OBJS)
	$(AR) rcs $@ $^


$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -IElement -IPlanet -c $< -o $@

$(OBJ_DIR)/Element/element.o: Element/element.h

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re