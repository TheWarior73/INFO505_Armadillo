#include "game.h"
#include "math.c"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int applyElementChange(Planet *planet, const Element *element)
{
	if (planet == NULL || element == NULL)
	{
		return 0;
	}

	switch (element->changeTarget)
	{
	case ELEMENT_CHANGE_POPULATION:
		return planetChangePopulation(planet, element->change.population);
	case ELEMENT_CHANGE_RADIUS:
	{
		double newRadius = planet->radiusKm + element->change.radiusKm;
		if (!isfinite(newRadius) || newRadius < MIN_PLANET_RADIUS || newRadius > MAX_PLANET_RADIUS)
		{
			return 0;
		}
		planet->radiusKm = newRadius;
		return 1;
	}
	case ELEMENT_CHANGE_WATER:
		return planetChangeResource(planet, PLANET_WATER, element->change.quantity);
	case ELEMENT_CHANGE_MINERALS:
		return planetChangeResource(planet, PLANET_MINERALS, element->change.quantity);
	case ELEMENT_CHANGE_WOOD:
		return planetChangeResource(planet, PLANET_WOOD, element->change.quantity);
	case ELEMENT_CHANGE_HABITABILITY:
	{
		long newHabitability = (long)planet->habitability + element->change.quantity;
		if (newHabitability < 0 || newHabitability > 100)
		{
			return 0;
		}
		return planetSetHabitability(planet, (int)newHabitability);
	}
	case ELEMENT_CHANGE_NONE:
	default:
		return 0;
	}
}

static int planetHasLost(const Planet *planet)
{
	return planet->population < MIN_POPULATION_ALLOWED ||
		   planet->water < MIN_WATER_ALLOWED ||
		   planet->wood < MIN_WOOD_ALLOWED ||
		   planet->minerals < MIN_MINERALS_ALLOWED ||
		   planet->habitability <= 0;
}

static int planetHasWon(const Planet *planet)
{
	return planet->population >= MAX_POPULATION_ALLOWED;
}

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 */
void runFullGame()
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
		Element *action = drawRandomElement(p->Actions);
		if (event == NULL && action == NULL)
		{
			printf("Game stopped: no events or actions are configured.\n");
			break;
		}

		long previousPopulation = p->population;
		double previousRadius = p->radiusKm;
		int previousWater = p->water;
		int previousMinerals = p->minerals;
		int previousWood = p->wood;
		int previousHabitability = p->habitability;

		gameTurn(p, event, action);
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

/**
 * Applies the event and action selected for the current turn.
 */
void gameTurn(Planet *p, Element *event, Element *action)
{
	if (p == NULL)
	{
		return;
	}

	applyElementChange(p, event);
	applyElementChange(p, action);
}

/**
 * @brief Initializes a planetwith random amounts of ressources, within playable parameters
 *
 * Playable parameters include :
 * - Planet radius must be at least 100km
 * - Population must be at least 2 to allow reproduction.
 * - Water, Minerals and Wood at total must be at least the amount of starting population.
 */
Planet *initPlanet()
{
	const char *planetName = NULL;
	double planetRadius = 0.0;
	long planetPopulation = 0;
	int water = 0, minerals = 0, wood = 0;

	// Possible Interactive Planet Setup Idea
	// getParametersFromPlayer(planetName, planetRadius, planetPopulation, water, minerals, wood);

	//
	// Individual usecases to check if a data is null and place a default value inside of it.
	//

	// Default planet name
	if (planetName == NULL)
	{
		planetName = "ArmadiLand";
	}

	// Default planet radius
	if (planetRadius == 0.0)
	{
		double random_value = rand() % MAX_DEFAULT_RADIUS;
		planetRadius = random_value;
	}
	planetRadius = clampDouble(planetRadius, MIN_PLANET_RADIUS, MAX_PLANET_RADIUS); // 100km to 10,000km of allowed radius

	// Default population
	if (planetPopulation == 0)
	{
		long random_value = rand() % MAX_DEFAULT_POPULATION;
		planetPopulation = random_value;
	}
	planetPopulation = clampLong(planetPopulation, MIN_POPULATION_ALLOWED, MAX_POPULATION_ALLOWED);

	// Default resources
	if (water == 0)
	{
		int random_value = rand() % MAX_DEFAULT_WATER;
		water = random_value;
	}
	if (wood == 0)
	{
		int random_value = rand() % MAX_DEFAULT_WOOD;
		wood = random_value;
	}
	if (minerals == 0)
	{
		int random_value = rand() % MAX_DEFAULT_MINERALS;
		minerals = random_value;
	}

	water = clampInt(water, MIN_WATER_ALLOWED, MAX_WATER_ALLOWED);
	wood = clampInt(wood, MIN_WOOD_ALLOWED, MAX_WOOD_ALLOWED);
	minerals = clampInt(minerals, MIN_MINERALS_ALLOWED, MAX_MINERALS_ALLOWED);

	// In case WatterWoodMinerals is inferior to population, apply a correction to bring back to at least the population level.
	long wwm = water + wood + minerals;
	if (wwm < planetPopulation)
	{
		long deficit = planetPopulation - wwm;

		water = water + (deficit / 3);
		wood = wood + (deficit / 3);
		minerals = minerals + (deficit / 3);
	}

	return createPlanet(planetName, planetRadius, planetPopulation, water, minerals, wood);
}