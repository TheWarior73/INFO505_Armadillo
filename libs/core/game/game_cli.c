#include "game.h"
#include "game.c"
#include <stdio.h>

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 */
void runCLIGame()
{
	Planet *p = initPlanet();
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