# Planet Manager - Armadillo

> Scope
>
> - INFO 505 @ USMB

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

