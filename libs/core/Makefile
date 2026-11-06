CC = gcc
AR = ar

GTK_CFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)
CFLAGS = -Wall -Wextra -Werror -std=c11
NAME_ELEMENT = libElement.a
NAME_PLANET = libPlanet.a
NAME_GAME = libGame.a

OBJ_DIR = build

OBJ_ELEMENT = $(OBJ_DIR)/Element/element.o $(OBJ_DIR)/Element/cJSON.o
OBJ_PLANET = $(OBJ_DIR)/Planet/planet.o
OBJ_GAME = $(OBJ_DIR)/game/game.o $(OBJ_DIR)/game/math.o $(OBJ_DIR)/game/game_cli.o $(OBJ_DIR)/game/game_gui.o

all: $(NAME_ELEMENT) $(NAME_PLANET) $(NAME_GAME)

$(NAME_ELEMENT): $(OBJ_ELEMENT)
	$(AR) rcs $@ $^

$(NAME_PLANET): $(OBJ_PLANET)
	$(AR) rcs $@ $^

$(NAME_GAME): $(OBJ_GAME)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -IElement -IPlanet -c $< -o $@

$(OBJ_DIR)/Element/element.o: Element/element.h

$(OBJ_DIR)/Planet/planet.o: Planet/planet.h Element/element.h

$(OBJ_DIR)/game/math.o: game/math.c

$(OBJ_DIR)/game/game.o: game/game.h game/math.c Planet/planet.h Element/element.h

$(OBJ_DIR)/game/game_cli.o: game/game.h game/game_cli.c

$(OBJ_DIR)/game/game_gui.o: game/game_gui.c game/game.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -IElement -IPlanet -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME_ELEMENT) $(NAME_PLANET) $(NAME_GAME)

re: fclean all

.PHONY: all clean fclean re