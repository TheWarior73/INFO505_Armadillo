#include "game.h"
#include "math.c"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*ElementChangeHandler)(Planet *, const Element *);

/**
 * @param planet: Pointer to the planet whose population to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
 * @brief Changes the population of the planet based on the element's change value.
 */
int changePopulation(Planet *planet, const Element *element)
{
	return planetChangePopulation(planet, element->change.population);
}

/**
 * @param planet: Pointer to the planet whose radius to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
 * @brief Changes the radius of the planet based on the element's change value.
 */
int changeRadius(Planet *planet, const Element *element)
	{
	double newRadius = planet->radiusKm + element->change.radiusKm;
	if (!isfinite(newRadius) || newRadius < MIN_PLANET_RADIUS || newRadius > MAX_PLANET_RADIUS)
	{
		return 0;
	}
	planet->radiusKm = newRadius;
	return 1;
}

/**
 * @param planet: Pointer to the planet whose resource to change
 * @param element: Pointer to the element containing the change value
 * @param resource: The type of resource to change
 * @return 1 if successful, 0 otherwise
 * @brief Changes a specific resource of the planet based on the element's change value.
 */
int changeResource(Planet *planet, const Element *element, PlanetResource resource)
{
	return planetChangeResource(planet, resource, element->change.quantity);
}

/**
 * @param planet: Pointer to the planet whose water to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
 * @brief Changes the water resource of the planet based on the element's change value.
 */
int changeWater(Planet *planet, const Element *element)
{
	return changeResource(planet, element, PLANET_WATER);
}

/**
 * @param planet: Pointer to the planet whose minerals to change
 * @param element: Pointer to the element containing the change value
 * @return 1 if successful, 0 otherwise
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
 * @return 1 if successful, 0 otherwise
 * @brief Changes the habitability of the planet based on the element's change value.
 */
int changeHabitability(Planet *planet, const Element *element)
{
	long newHabitability = (long)planet->habitability + element->change.quantity;
	if (newHabitability < 0 || newHabitability > 100)
	{
		return 0;
	}
	return planetSetHabitability(planet, (int)newHabitability);
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
static int applyElementChange(Planet *planet, const Element *element)
{
	if (planet == NULL || element == NULL || element->changeTarget < ELEMENT_CHANGE_NONE ||
		element->changeTarget > ELEMENT_CHANGE_HABITABILITY)
	{
		return 0;
	}

	ElementChangeHandler handler = changeHandlers[element->changeTarget];
	return handler != NULL ? handler(planet, element) : 0;
}

/**
 * @param planet: Pointer to the planet to check
 * @return 1 if the planet has lost, 0 otherwise
 * @brief Checks if the planet has lost based on its properties.
 */
static int planetHasLost(const Planet *planet)
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