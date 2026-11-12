#ifndef GAME_C
#define GAME_C

#include "game.h"
#include "math.c"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*ElementChangeHandler)(Planet *, const Element *);

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

	if (action != NULL)
	{
		FILE *history = fopen(GAME_HISTORY_FILE, "a");
		if (history != NULL)
		{
			fprintf(history, "Event: %s | Action: %s\n", event != NULL ? event->nameElement : "None", action->nameElement);
			fclose(history);
		}
		else
		{
			perror("Unable to append game history");
		}
	}
}

/**
 * @param planet: Pointer to the planet whose population to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, -1 otherwise
 * @brief Changes the population of the planet based on the element's change value.
 */
int changePopulation(Planet *planet, const Element *element)
{
	return planetChangePopulation(planet, element->change.population);
}

/**
 * @param planet: Pointer to the planet whose radius to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, -1 otherwise
 * @brief Changes the radius of the planet based on the element's change value.
 */
int changeRadius(Planet *planet, const Element *element)
{
	double newRadius = planet->radiusKm + element->change.radiusKm;
	if (!isfinite(newRadius) || newRadius < MIN_PLANET_RADIUS || newRadius > MAX_PLANET_RADIUS)
	{
		return -1;
	}
	planet->radiusKm = newRadius;
	return 1;
}

/**
 * @param planet: Pointer to the planet whose resource to change
 * @param element: Pointer to the element containing the change value
 * @param resource: The type of resource to change
 * @return 1 if successful, -1 otherwise
 * @brief Changes a specific resource of the planet based on the element's change value.
 */
int changeResource(Planet *planet, const Element *element, PlanetResource resource)
{
	return planetChangeResource(planet, resource, element->change.quantity);
}

/**
 * @param planet: Pointer to the planet whose water to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, -1 otherwise
 * @brief Changes the water resource of the planet based on the element's change value.
 */
int changeWater(Planet *planet, const Element *element)
{
	return changeResource(planet, element, PLANET_WATER);
}

/**
 * @param planet: Pointer to the planet whose minerals to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, -1 otherwise
 * @brief Changes the minerals resource of the planet based on the element's change value.
 */
int changeMinerals(Planet *planet, const Element *element)
{
	return changeResource(planet, element, PLANET_MINERALS);
}

/**
 * @param planet: Pointer to the planet whose wood to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
 * @brief Changes the wood resource of the planet based on the element's change value.
 */
int changeWood(Planet *planet, const Element *element)
{
	return changeResource(planet, element, PLANET_WOOD);
}

/**
 * @param planet: Pointer to the planet whose habitability to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, -1 otherwise
 * @brief Changes the habitability of the planet based on the element's change value.
 */
int changeHabitability(Planet *planet, const Element *element)
{
	int newHabitability = planet->habitability + element->change.quantity;
	if (newHabitability < 0 || newHabitability > 100)
	{
		return planetSetHabitability(planet, clampInt(newHabitability, 0, 200));
	}

	return planetSetHabitability(planet, newHabitability);
}

static const ElementChangeHandler changeHandlers[] = {
	[ELEMENT_CHANGE_POPULATION] = changePopulation,
	[ELEMENT_CHANGE_RADIUS] = changeRadius,
	[ELEMENT_CHANGE_WATER] = changeWater,
	[ELEMENT_CHANGE_MINERALS] = changeMinerals,
	[ELEMENT_CHANGE_WOOD] = changeWood,
	[ELEMENT_CHANGE_HABITABILITY] = changeHabitability,
};

/**
 * @param planet: Pointer to the planet whose resource to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
 * @brief Applies the change specified by the element to the planet.
 */
int applyElementChange(Planet *planet, const Element *element)
{
	if (planet == NULL || element == NULL || element->changeTarget < ELEMENT_CHANGE_NONE ||
		element->changeTarget > ELEMENT_CHANGE_HABITABILITY)
	{
		return -1;
	}

	ElementChangeHandler handler = changeHandlers[element->changeTarget];
	return handler != NULL ? handler(planet, element) : -1;
}

/**
 * @param planet: Pointer to the planet to check
 * @return 1 if the planet has lost, 0 otherwise
 * @brief Checks if the planet has lost based on its properties.
 */
int planetHasLost(const Planet *planet)
{
	return planet->population < MIN_POPULATION_ALLOWED ||
		planet->water < MIN_WATER_ALLOWED ||
		planet->wood < MIN_WOOD_ALLOWED ||
		planet->minerals < MIN_MINERALS_ALLOWED ||
		planet->habitability <= 0;
}

/**
 * @param planet: Pointer to the planet to check
 * @return 1 if the planet has won, 0 otherwise
 * @brief Checks if the planet has won based on its properties.
 */
int planetHasWon(const Planet *planet)
{
	return planet->population >= MAX_POPULATION_ALLOWED;
}

/**
 * @brief Initializes a planetwith random amounts of ressources, within playable parameters
 *
 * Playable parameters include :
 * - Planet radius must be at least 100km
 * - Population must be at least 2 to allow reproduction.
 * - Water, Minerals and Wood at total must be at least the amount of starting population.
 */
Planet *initPlanetWithParameters(const char *planetName, double planetRadius, long planetPopulation, int water, int minerals, int wood)
{
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

	Planet *planet = createPlanet(planetName, planetRadius, planetPopulation, water, minerals, wood);
	if (planet == NULL)
	{
		return NULL;
	}

	planet->Events = fetchEventsFromFile("../data/climaticElements.jsonc", "Event");
	planet->Actions = fetchEventsFromFile("../data/climaticElements.jsonc", "Action");
	if (planet->Events == NULL || planet->Actions == NULL)
	{
		fprintf(stderr, "Unable to load events and actions from ../data/climaticElements.jsonc.\n");
		destroyPlanet(planet);
		return NULL;
	}

	return planet;
}

Planet *initPlanet()
{
	return initPlanetWithParameters(NULL, 0.0, 0, 0, 0, 0);
}

#endif