#include "planet.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param name The name of the planet.
 * @param radiusKm The radius of the planet in kilometers.
 * @param population The population of the planet.
 * @param water The amount of water resources.
 * @param minerals The amount of mineral resources.
 * @param wood The amount of wood resources.
 * @return A pointer to the newly created planet, or NULL if creation failed.
 * @brief Creates a new planet with the specified properties.
 */
Planet *createPlanet(const char *name, double radiusKm, long population, int water, int minerals, int wood)
{
	if (name == NULL || name[0] == '\0' || !isfinite(radiusKm) || radiusKm <= 0.0 ||
		population < 0 || water < 0 || minerals < 0 || wood < 0)
	{
		return NULL;
	}

	Planet *planet = malloc(sizeof(*planet));
	if (planet == NULL)
	{
		return NULL;
	}

	size_t nameSize = strlen(name) + 1;
	planet->name = malloc(nameSize);
	if (planet->name == NULL)
	{
		free(planet);
		return NULL;
	}

	memcpy(planet->name, name, nameSize);
	planet->radiusKm = radiusKm;
	planet->population = population;
	planet->water = water;
	planet->minerals = minerals;
	planet->wood = wood;
	planet->habitability = 100;
	return planet;
}

/**
 * @param planet: Pointer to the planet to add an action to
 * @param nameAction: The name of the action to add
 * @param nameType: The type of the action to add
 * @brief Adds an action to a planet
 */
void AddActionToPlanet(Planet *planet, const char *nameAction, const char *nameType)
{
	if (planet == NULL || nameAction == NULL || nameType == NULL)
	{
		return;
	}
	addElement(&planet->Actions, nameAction, nameType);
}

/**
 * @param planet: Pointer to the planet to add an event to
 * @param nameEvent: The name of the event to add
 * @param nameType: The type of the event to add
 * @brief Adds an event to a planet
 */
void AddEventToPlanet(Planet *planet, const char *nameEvent, const char *nameType)
{
	if (planet == NULL || nameEvent == NULL || nameType == NULL)
	{
		return;
	}
	addElement(&planet->Events, nameEvent, nameType);
}

/**
 * @param planet: Pointer to the planet to destroy
 * @brief Destroys a planet and frees its memory
 */
void destroyPlanet(Planet *planet)
{
	if (planet == NULL)
	{
		return;
	}

	free(planet->name);
	freeListElements(planet->Actions);
	freeListElements(planet->Events);
	free(planet);
}

/**
 * @param planet: Pointer to the planet whose population to change
 * @param change: The amount to change the population by
 * @return 1 if successful, 0 otherwise
 * @brief Changes the population of a planet
 */
int planetChangePopulation(Planet *planet, long change)
{
	if (planet == NULL || (change > 0 && planet->population > LONG_MAX - change) ||
		(change < 0 && change < -planet->population))
	{
		return 0;
	}

	planet->population += change;
	return 1;
}

/**
 * @param planet: Pointer to the planet whose resource to change
 * @param resource: The type of resource to change
 * @param change: The amount to change the resource by
 * @return 1 if successful, 0 otherwise
 * @brief Changes a resource of a planet
 */
int planetChangeResource(Planet *planet, PlanetResource resource, int change)
{
	if (planet == NULL)
	{
		return 0;
	}

	int *quantity;
	if (resource == PLANET_WATER)
	{
		quantity = &planet->water;
	}
	else if (resource == PLANET_MINERALS)
	{
		quantity = &planet->minerals;
	}
	else if (resource == PLANET_WOOD)
	{
		quantity = &planet->wood;
	}
	else
	{
		return 0;
	}

	if ((change > 0 && *quantity > INT_MAX - change) ||
		(change < 0 && change < -*quantity))
	{
		return 0;
	}

	*quantity += change;
	return 1;
}

/**
 * @param planet: Pointer to the planet whose habitability to set
 * @param habitability: The new habitability value
 * @return 1 if successful, 0 otherwise
 * @brief Sets the habitability of a planet
 */
int planetSetHabitability(Planet *planet, int habitability)
{
	if (planet == NULL || habitability < 0 || habitability > 100)
	{
		return 0;
	}

	planet->habitability = habitability;
	return 1;
}

/**
 * @param planet: Pointer to the planet to display
 * @brief Displays the information of a planet
 */
void displayPlanet(const Planet *planet)
{
	if (planet == NULL)
	{
		printf("No planet to display.\n");
		return;
	}

	printf("Planet: %-15s", planet->name);
	printf("Radius: %.2f%-10s", planet->radiusKm, "km");
	printf("Population: %-15ld\n", planet->population);
	printf("Water: %d | Minerals: %d | Wood: %d\n", planet->water, planet->minerals, planet->wood);
	printf("Habitability: %d/100\n", planet->habitability);
	printf("Actions:\n");
	displayElements(planet->Actions);
	printf("Events:\n");
	displayElements(planet->Events);
}

/**
 * @param planet: Pointer to the planet to verify
 * @brief Verifies the information of a planet and displays warnings if any values are out of expected ranges
 */
void VerifyPlanet(const Planet *planet)
{
	if (planet == NULL)
	{
		printf("No planet to verify.\n");
		return;
	}

	if (planet->population < 0)
	{
		printf("Warning: Population is negative.\n");
	}
	if (planet->water < 0)
	{
		printf("Warning: Water resources are negative.\n");
	}
	if (planet->minerals < 0)
	{
		printf("Warning: Mineral resources are negative.\n");
	}
	if (planet->wood < 0)
	{
		printf("Warning: Wood resources are negative.\n");
	}
	if (planet->habitability < 0 || planet->habitability > 100)
	{
		printf("Warning: Habitability is out of bounds (0-100).\n");
	}

	displayPlanet(planet);
}