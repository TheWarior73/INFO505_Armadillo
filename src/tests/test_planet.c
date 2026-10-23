#include <assert.h>
#include <limits.h>
#include <string.h>

#include "../../libs/core/Planet/planet.h"

static void test_create_and_destroy_planet(void)
{
	Planet *planet = createPlanet("Armadillo", 6371.0, 1000, 500, 250, 100);

	assert(planet != NULL);
	displayPlanet(planet);
	assert(strcmp(planet->name, "Armadillo") == 0);
	assert(planet->radiusKm == 6371.0);
	assert(planet->population == 1000);
	assert(planet->water == 500);
	assert(planet->minerals == 250);
	assert(planet->wood == 100);
	assert(planet->habitability == 100);
	assert(planet->Actions == NULL);
	assert(planet->Events == NULL);

	destroyPlanet(planet);
	destroyPlanet(NULL);
	assert(createPlanet(NULL, 1.0, 0, 0, 0, 0) == NULL);
	assert(createPlanet("", 1.0, 0, 0, 0, 0) == NULL);
	assert(createPlanet("Invalid", 0.0, 0, 0, 0, 0) == NULL);
	assert(createPlanet("Invalid", 1.0, -1, 0, 0, 0) == NULL);
}

static void test_add_action_to_planet(void)
{
	Planet *planet = createPlanet("Armadillo", 1000.0, 10, 20, 30, 100);
	ElementChangeValue change = {.quantity = -5};
	Element *action = createElement("Drought", "event", ELEMENT_CHANGE_WATER, change);

	assert(planet != NULL);
	assert(action != NULL);
	AddActionToPlanet(planet, action);
	assert(planet->Actions == action);
	assert(planet->Actions->changeTarget == ELEMENT_CHANGE_WATER);
	assert(planet->Actions->change.quantity == -5);

	AddActionToPlanet(planet, NULL);
	assert(planet->Actions == action);
	destroyPlanet(planet);
}

static void test_planet_changes(void)
{
	Planet *planet = createPlanet("Armadillo", 1000.0, 10, 20, 30, 100);
	assert(planet != NULL);

	assert(planetChangePopulation(planet, 5) == 1);
	assert(planet->population == 15);
	assert(planetChangePopulation(planet, -15) == 1);
	assert(planet->population == 0);
	assert(planetChangePopulation(planet, -1) == 0);
	planet->population = LONG_MAX;
	assert(planetChangePopulation(planet, 1) == 0);

	assert(planetChangeResource(planet, PLANET_WATER, -5) == 1);
	assert(planet->water == 15);
	assert(planetChangeResource(planet, PLANET_MINERALS, 10) == 1);
	assert(planet->minerals == 40);
	assert(planetChangeResource(planet, PLANET_WOOD, -5) == 1);
	assert(planet->wood == 95);
	assert(planetChangeResource(planet, PLANET_WATER, -16) == 0);
	assert(planetChangeResource(planet, (PlanetResource)99, 1) == 0);

	assert(planetSetHabitability(planet, 75) == 1);
	assert(planet->habitability == 75);
	assert(planetSetHabitability(planet, 101) == 0);
	assert(planetSetHabitability(NULL, 50) == 0);

	destroyPlanet(planet);
}

void test_planet(void)
{
	test_create_and_destroy_planet();
	test_add_action_to_planet();
	test_planet_changes();
}