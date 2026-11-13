#ifndef GAME_H
#define GAME_H

#include "../Planet/planet.h"
#include "../Element/element.h"

#define MAX_DEFAULT_RADIUS 6351			   // earth radius in km
#define MIN_PLANET_RADIUS 100			   // min radius in km
#define MAX_PLANET_RADIUS 10000			   // max radius in km
										   //
#define MAX_DEFAULT_POPULATION 100000	   // 100k: Maximum default population allowed
#define MIN_POPULATION_ALLOWED 2		   // Minimum population allowed before Game Over (being 2 to allow reproduction, gender will not be accounted for here)
#define MAX_POPULATION_ALLOWED 10000000000 // 10 Billion max pop. allowed, will be clamped.
										   //
#define MAX_DEFAULT_WATER 10000000		   // 10M: MAX starting water
#define MIN_WATER_ALLOWED 100000		   // Bellow that threshold, the life becomes unsustainable due to global water shortages. (loose condition)
#define MAX_WATER_ALLOWED 15000000		   // 15M: Will make the game consider the world flooded (loose condition)
										   //
#define MAX_DEFAULT_WOOD 10000000		   // 10M: MAX starting wood
#define MIN_WOOD_ALLOWED 100000			   // Bellow that threshold, the life becomes unsustainable due to ecosystem collapse (loose condition)
#define MAX_WOOD_ALLOWED 15000000		   // Will make the game consider the fauna to be out of control (loose condition)
										   //
#define MAX_DEFAULT_MINERALS 10000000	   // 10M: MAX starting minerals
#define MIN_MINERALS_ALLOWED 100000		   // Bellow that threshold, the life becomes unsustainable due to ressource shortage (loose condition)
#define MAX_MINERALS_ALLOWED 1000000000	   // Will be clamped.
										   //
#define WIN_POPULATION_THRESHOLD 1000000   // 1M: Has to have at least a million population to be eligible to win
#define WIN_HABITABILITY_THRESHOLD 75	   // 75%: Has to have at least 75 habitability to win
#define WIN_MIN_WOOD 2000000			   // 2M: Min ressource needed to be able to win
#define WIN_MIN_WATER 2000000			   // 2M: Min ressource needed to be able to win
#define WIN_MIN_MINERALS 2000000		   // 2M: Min ressource needed to be able to win

#define GAME_HISTORY_FILE "../data/game_history.log"

void runCLIGame();

void gameTurn(Planet *p, Element *event, Element *action);
Planet *initPlanet();
Planet *initPlanetWithParameters(const char *planetName, double planetRadius, long planetPopulation, int water, int minerals, int wood);
void getParametersFromPlayer(char *name, double *radius, long *population, int *water, int *minerals, int *wood);

int changePopulation(Planet *planet, const Element *element);
int changeRadius(Planet *planet, const Element *element);
int changeResource(Planet *planet, const Element *element, PlanetResource resource);
int changeWater(Planet *planet, const Element *element);
int changeMinerals(Planet *planet, const Element *element);
int changeWood(Planet *planet, const Element *element);
int changeHabitability(Planet *planet, const Element *element);

int planetHasWon(const Planet *planet);
int planetHasLost(const Planet *planet);
int applyElementChange(Planet *planet, const Element *element);

#endif