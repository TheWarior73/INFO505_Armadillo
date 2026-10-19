#include "planet.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void destroyPlanet(Planet *planet)
{
	if (planet == NULL)
	{
		return;
	}

	free(planet->name);
	free(planet);
}

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

int planetSetHabitability(Planet *planet, int habitability)
{
	if (planet == NULL || habitability < 0 || habitability > 100)
	{
		return 0;
	}

	planet->habitability = habitability;
	return 1;
}

void displayPlanet(const Planet *planet)
{
	if (planet == NULL)
	{
		printf("No planet to display.\n");
		return;
	}

	printf("Planet: %s\n", planet->name);
	printf("Radius: %.2f km\n", planet->radiusKm);
	printf("Population: %ld\n", planet->population);
	printf("Water: %d | Minerals: %d | Wood: %d\n", planet->water, planet->minerals, planet->wood);
	printf("Habitability: %d/100\n", planet->habitability);
}