#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../libs/core/game/game.h"

/**
 * @brief Defines wether the output should be verbose or not.
 */
int is_verbose = 0;

/**
 * @brief Defines wether the program should run in debug mode or not (run tests)
 */
int is_debug = 0;
int is_game = 0;
int is_gui = 0;

//

void test_liste_chainee(void);
void test_planet(void);
void test_load_liste_element(void);
void print_help(void);

int main(int argc, char *argv[])
{
	// Seeding the randomizer
	srand(time(NULL));

	if (argc == 1)
	{
		print_help();
		return EXIT_FAILURE;
	}

	// Loop through the arguments to set the appropriate flags
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			print_help();
			return EXIT_SUCCESS;
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
		{
			is_verbose = 1;
		}
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
		{
			is_debug = 1;
		}
		else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gui") == 0)
		{
			is_game = 1;
			is_gui = 1;
		}
		else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--cli") == 0)
		{
			is_game = 1;
		}
		else if (i == argc - 1)
		{
			printf("Invalid Args, use -h or --help to see available flags\n");
			return EXIT_FAILURE;
		}
	}

	if (is_verbose)
	{
		printf("=================================\n"
			   "Running program in verbose mode !\n"
			   "=================================\n");
	}

	if (is_debug)
	{
		test_liste_chainee();
		test_planet();
		test_load_liste_element();
	}

	if (is_game)
	{
		if (is_gui)
		{
			runGUIGame(argc, argv);
		}
		else
		{
			runCLIGame();
		}
	}

	return EXIT_SUCCESS;
}

/**
 * @brief Prints an help message to explain the available flags
 */
void print_help(void)
{
	printf("List of available flags:\n"
		   "\t-v \tPrints more information to the console\n"
		   "\t--verbose\n"
		   "\n"
		   "\t-d \tRuns the program in debug mode (runs tests)\n"
		   "\t--debug\n"
		   "\n"
		   "\t-c \tRuns the program in CLI mode\n"
		   "\t--cli\n"
		   "\n"
		   "\t-g \tRuns the program in GUI mode\n"
		   "\t--gui\n");
}