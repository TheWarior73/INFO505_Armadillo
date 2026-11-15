# Planet Manager - Armadillo

> Scope
>
> - INFO 505 @ USMB

## Game information

### Win conditions

To win the game you must have the following requirements :

- At least 1M population
- At least 75% planet habitability
- At least 1M water
- At least 1M wood
- At least 1M minerals

### Loose conditions

To loose the game, any of the following requirements must be met :

- Less than 2 population
- Less than 100k water
- Less than 100k wood
- Less than 100k minerals
- Planet habitability has reached 0
- Planet radius has become too small to sustain life.

Additionally, you can loose the game by having too much of some ressources :

- More than 15M Water : the world is flooded
- More than 15M Wood : the world is overgrown by mega fauna

## Turn information

A turn starts with a random event drawn by the game, the player then plays an action to infere with the ressources, population, habitability...

Each turn, the planet will loose ressources passively. It scales with the population amount.  
One person (1 pop.) consumes each turn :

- 1 water
- 0.5 wood
- 0.5 minerals

## Project information

### Structure

```
..
├── docs
│   ├── html
│   └── man
├── Doxyfile
├── libs
│   └── core
│       ├── Element
│       │   ├── cJSON.c   <-- External Lib
│       │   ├── cJSON.h   <-- External Lib
│       │   ├── element.c
│       │   └── element.h
│       ├── Game
│       │   ├── game_cli.c
│       │   ├── game_gui.c
│       │   ├── game.c
│       │   ├── game.h
│       │   └── math.c
│       ├── Planet
│       │   ├── planet.c
│       │   └── planet.h
│       └── Makefile
├── README.md
└── src
    ├── main.c
    ├── Makefile
    └── tests
        ├── test_element.c
        └── test_planet.c
```

### How to run

1. `$ cd ./src`
2. `$ make`
3. `$ ./armadillo.game --gui` to play in gui mode, or `$ ./armadillo.game --cli` to play in cli mode, or `$ ./armadillo.game --debug` to run the tests

## Contributing

Contributing is welcomed, see how to do that bellow

### Setting up your environment

- have clangd installed

The file `compile_commands.json` has the nescessary information for clangd to suppress error messages about GTK.

if not, at root of the repo, run :

`$ bear -- make -C src`

**Installing bear**

Arch - `# pacman -Sy bear`

Debian/Ubuntu - `# apt install bear`

### Running the GUI with the GTK Debuger

From the root of the project

`$ GTK_DEBUG=interactive src/armadillo.game --gui`

## Credits

- [DaveGamble's cJSON](https://github.com/davegamble/cjson) : Minimal Json parsing library in C.

