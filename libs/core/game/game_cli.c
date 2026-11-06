#include "game.h"
#include "game.c"
#include <stdio.h>

static void discardInputLine(void)
{
	int character;
	while ((character = getchar()) != '\n' && character != EOF)
	{
	}
}

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 */
void runCLIGame()
{
	char planetName[128] = {0};
	double planetRadius = 0.0;
	long planetPopulation = 0;
	int water = 0;
	int minerals = 0;
	int wood = 0;
	getParametersFromPlayer(planetName, &planetRadius, &planetPopulation, &water, &minerals, &wood);

	Planet *p = initPlanetWithParameters(planetName[0] != '\0' ? planetName : NULL,
		planetRadius, planetPopulation, water, minerals, wood);
	if (p == NULL)
	{
		fprintf(stderr, "Unable to initialize the planet.\n");
		return;
	}

	displayPlanet(p);

	while (!planetHasWon(p) && !planetHasLost(p))
	{
		Element *event = drawRandomElement(p->Events);
		Element *actionChoices[3] = {
			drawRandomElement(p->Actions),
			drawRandomElement(p->Actions),
			drawRandomElement(p->Actions)};
		if (actionChoices[0] == NULL)
		{
			printf("Game stopped: no actions are configured.\n");
			break;
		}

		printf("\nEvent: %s\n", event != NULL ? event->nameElement : "No event");
		printf("Choose an action:\n");
		for (int i = 0; i < 3; i++)
		{
			printf("%d. %s\n", i + 1, actionChoices[i]->nameElement);
		}

		int selectedChoice;
		printf("Your choice (1-3): ");
		if (scanf("%d", &selectedChoice) != 1 || selectedChoice < 1 || selectedChoice > 3)
		{
			printf("Game stopped: please enter a number between 1 and 3.\n");
			break;
		}

		long previousPopulation = p->population;
		double previousRadius = p->radiusKm;
		int previousWater = p->water;
		int previousMinerals = p->minerals;
		int previousWood = p->wood;
		int previousHabitability = p->habitability;

		gameTurn(p, event, actionChoices[selectedChoice - 1]);
		if (p->population == previousPopulation && p->radiusKm == previousRadius &&
			p->water == previousWater && p->minerals == previousMinerals &&
			p->wood == previousWood && p->habitability == previousHabitability)
		{
			printf("Game stopped: the selected action and event did not change the planet.\n");
			break;
		}
	}

	if (planetHasWon(p))
	{
		printf("Victory: the population reached the maximum allowed.\n");
	}
	else if (planetHasLost(p))
	{
		printf("Defeat: the planet no longer meets its survival thresholds.\n");
	}

	displayPlanet(p);
	destroyPlanet(p);
}

void getParametersFromPlayer(char *name, double *radius, long *population, int *water, int *minerals, int *wood)
{
	name[0] = '\0';
	*radius = 0.0;
	*population = 0;
	*water = 0;
	*minerals = 0;
	*wood = 0;

	printf("Enter planet name: ");
	if (scanf("%127[^\n]", name) != 1)
	{
		name[0] = '\0';
	}
	discardInputLine();

	printf("Enter planet radius (km): ");
	if (scanf("%lf", radius) != 1)
	{
		*radius = 0.0;
	}
	discardInputLine();

	printf("Enter planet population: ");
	if (scanf("%ld", population) != 1)
	{
		*population = 0;
	}
	discardInputLine();

	printf("Enter water quantity: ");
	if (scanf("%d", water) != 1)
	{
		*water = 0;
	}
	discardInputLine();

	printf("Enter minerals quantity: ");
	if (scanf("%d", minerals) != 1)
	{
		*minerals = 0;
	}
	discardInputLine();

	printf("Enter wood quantity: ");
	if (scanf("%d", wood) != 1)
	{
		*wood = 0;
	}
	discardInputLine();
}