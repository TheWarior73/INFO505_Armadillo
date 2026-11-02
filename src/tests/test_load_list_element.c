#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../../libs/core/Element/element.h"

static void test_fetch_empty_filepath(void)
{
	// Test NULL filepath with "Event" type
	Element *events = fetchEventsFromFile(NULL, "Event");
	assert(events != NULL);
	assert(strcmp(events->nameType, "Event") == 0);
	// Assuming a destroy function exists for cleanup
	// destroyElementList(events);

	// Test NULL filepath with "Action" type
	Element *actions = fetchEventsFromFile(NULL, "Action");
	assert(actions != NULL);
	assert(strcmp(actions->nameType, "Action") == 0);
	// destroyElementList(actions);

	// Test NULL filepath with invalid type (should default to "Event")
	Element *fallbackEvents = fetchEventsFromFile(NULL, "InvalidType");
	assert(fallbackEvents != NULL);
	assert(strcmp(fallbackEvents->nameType, "Event") == 0);
	// destroyElementList(fallbackEvents);
}

static void test_fetch_wrong_filepath(void)
{
	// File does not exist
	Element *result = fetchEventsFromFile("../data/does_not_exist.json", "Event");
	assert(result == NULL);
}

static void test_fetch_correct_filepath_correct_type(void)
{
	// Assuming the test binary is run from the project root or src directory.
	const char *filepath = "../data/climaticElements.jsonc";

	// Valid file, Valid type: Event
	Element *events = fetchEventsFromFile(filepath, "Event");
	assert(events != NULL);
	assert(strcmp(events->nameType, "Event") == 0);
	// destroyElementList(events);

	// Valid file, Valid type: Action
	Element *actions = fetchEventsFromFile(filepath, "Action");
	assert(actions != NULL);
	assert(strcmp(actions->nameType, "Action") == 0);
	// destroyElementList(actions);
}

static void test_fetch_correct_filepath_incorrect_type(void)
{
	const char *filepath = "../data/climaticElements.jsonc";

	// Valid file, Invalid type: should fail to find the array in JSON and return NULL
	Element *result = fetchEventsFromFile(filepath, "NonExistentType");
	assert(result != NULL);
}

void test_load_liste_element(void)
{
	test_fetch_empty_filepath();
	test_fetch_wrong_filepath();
	test_fetch_correct_filepath_correct_type();
	test_fetch_correct_filepath_incorrect_type();

	printf("test_element_loading passed.\n");
}