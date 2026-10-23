#include "game.h"
#include "math.c"

#include <stdlib.h>
#include <string.h>

/**
 * This function will initialize everything to a playable state,
 * before launching the game loop, once the loop stops it will handle
 * memory deallocation as well.
 *
 * @brief Runs the full game, turn by turn, until win condition or game over is reached.
 */
void runFullGame()
{
	// Planet *p = initPlanet();
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
	const char *planetName;
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

	return createPlanet(planetName, planetRadius, planetPopulation, water, minerals, wood);
}