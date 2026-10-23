#ifndef GAME_H
#define GAME_H

#include "../Planet/planet.h"
#include "../Element/element.h"

#define MAX_DEFAULT_RADIUS 6351			   // earth radius in km
#define MIN_PLANET_RADIUS 100			   // min radius in km
#define MAX_PLANET_RADIUS 10000			   // max radius in km
										   //
#define MAX_DEFAULT_POPULATION 100000	   // Maximum default population allowed
#define MIN_POPULATION_ALLOWED 2		   // Minimum population allowed before Game Over (being 2 to allow reproduction, gender will not be accounted for here)
#define MAX_POPULATION_ALLOWED 10000000000 // 10 Billion max pop. allowed, the effects of going over this limit are not yet decided
										   //
#define MAX_DEFAULT_WATER 10000000		   // 10 Million MAX starting water
#define MIN_WATER_ALLOWED 1				   // Bellow that threshold, the life becomes unsustainable regardless of conditions
#define MAX_WATER_ALLOWED 1000000000	   // Will either be clamped or make the game consider the world flooded (loose condition) (too be decided) @TODO
										   //
#define MAX_DEFAULT_WOOD 10000000		   // 10 Million MAX starting wood
#define MIN_WOOD_ALLOWED 1				   // Bellow that threshold, the life becomes unsustainable regardless of conditions
#define MAX_WOOD_ALLOWED 1000000000		   // Will either be clamped or make the game consider the tree mass extinct (loose condition) (to be decided) @TODO
										   //
#define MAX_DEFAULT_MINERALS 10000000	   // 10 Million MAX starting minerals
#define MIN_MINERALS_ALLOWED 1			   // Bellow that threshold, the life becomes unsustainable regardless of conditions
#define MAX_MINERALS_ALLOWED 1000000000	   // Will be clamped.

void runFullGame();
void gameTurn(Planet *p, Element *event, Element *action);
Planet *initPlanet();

#endif