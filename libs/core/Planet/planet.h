#ifndef PLANET_H
#define PLANET_H

typedef enum
{
	PLANET_WATER,
	PLANET_MINERALS,
    PLANET_WOOD
} PlanetResource;

typedef struct
{
	char *name;
	double radiusKm;
	long population;
	int water;
	int minerals;
    int wood;
	int habitability;
} Planet;

Planet *createPlanet(const char *name, double radiusKm, long population, int water, int minerals, int wood);
void destroyPlanet(Planet *planet);
int planetChangePopulation(Planet *planet, long change);
int planetChangeResource(Planet *planet, PlanetResource resource, int change);
int planetSetHabitability(Planet *planet, int habitability);
void displayPlanet(const Planet *planet);

#endif