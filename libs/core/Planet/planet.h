#ifndef PLANET_H
#define PLANET_H

#ifndef ELEMENT_H
#include "../Element/element.h"
#endif

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
	Element *Actions;
	Element *Events;
} Planet;

Planet *createPlanet(const char *name, double radiusKm, long population, int water, int minerals, int wood);
void AddActionToPlanet(Planet *planet, const char *nameAction, const char *nameType);
void AddEventToPlanet(Planet *planet, const char *nameEvent, const char *nameType);
void destroyPlanet(Planet *planet);
int planetChangePopulation(Planet *planet, long change);
int planetChangeResource(Planet *planet, PlanetResource resource, int change);
int planetSetHabitability(Planet *planet, int habitability);
void displayPlanet(const Planet *planet);
void VerifyPlanet(const Planet *planet);

#endif