#include "element.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param nameElement: Name of the element to create
 * @param nameType: Type of the element to create
 * @return A pointer to the newly created element, or NULL if creation failed
 * @brief Creates a new element with the specified properties
 */
Element *createElement(const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change)
{
	if (nameElement == NULL || nameType == NULL)
	{
		return NULL;
	}

	Element *element = malloc(sizeof(*element));
	if (element == NULL)
	{
		return NULL;
	}

	size_t elementNameSize = strlen(nameElement) + 1;
	size_t typeNameSize = strlen(nameType) + 1;
	element->nameElement = malloc(elementNameSize);
	element->nameType = malloc(typeNameSize);
	if (element->nameElement == NULL || element->nameType == NULL)
	{
		free(element->nameElement);
		free(element->nameType);
		free(element);
		return NULL;
	}

	memcpy(element->nameElement, nameElement, elementNameSize);
	memcpy(element->nameType, nameType, typeNameSize);
	element->changeTarget = target;
	element->change = change;
	element->next = NULL;
	return element;
}

/**
 * @param list: element list pointer
 * @param nameElement: Name of the element to add
 * @param nameType: Type of the element to add
 * @param target: The target property of the planet that this element will change
 * @param change: The value of the change that this element will apply to the target property
 * @brief Adds an element carrying a signed change for a planet property to the beginning of the list
 */
void addElement(Element **list, const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change)
{
	if (list == NULL || target <= ELEMENT_CHANGE_NONE || target > ELEMENT_CHANGE_HABITABILITY)
	{
		return;
	}

	Element *newElement = createElement(nameElement, nameType, target, change);
	if (newElement == NULL)
	{
		return;
	}

	newElement->next = *list;
	*list = newElement;
}

/**
 * @param list: element list pointer
 * @param nameElement: Name of the element to add
 * @param nameType: Type of the element to add
 * @param target: The target property of the planet that this element will change
 * @param change: The value of the change that this element will apply to the target property
 * @brief Adds an element to the end of the list
 */
void addElementTail(Element **list, const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change)
{
	if (list == NULL)
	{
		return;
	}

	Element *nouvelElement = createElement(nameElement, nameType, target, change);
	if (nouvelElement == NULL)
	{
		return;
	}

	if (*list == NULL)
	{
		*list = nouvelElement;
		return;
	}

	Element *current = *list;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = nouvelElement;
}

/**
 * @param list: element list pointer
 * @param nameElement: Name of the element to delete
 * @brief Deletes first element with the given name from the list
 */
void deleteElement(Element **list, const char *nameElement)
{
	if (list == NULL || *list == NULL || nameElement == NULL)
	{
		return;
	}

	Element *current = *list;
	Element *previous = NULL;

	while (current != NULL)
	{
		if (strcmp(current->nameElement, nameElement) == 0)
		{
			if (previous == NULL)
			{
				*list = current->next; // first element case
			}
			else
			{
				previous->next = current->next; // other elements case
			}
			free(current->nameElement);
			free(current->nameType);
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

/**
 * @param list: element list pointer
 * @brief Displays all elements in the list
 */
void displayElements(const Element *list)
{
	if (list == NULL)
	{
		printf("The list is empty.\n");
		return;
	}

	const Element *current = list;
	while (current != NULL)
	{
		printf("Element name: %-10s Type: %s\n", current->nameElement, current->nameType);
		current = current->next;
	}
}

/**
 * @param list: element list pointer
 * @brief Frees all elements in the list
 */
void freeListElements(Element *list)
{
	Element *current = list;
	while (current != NULL)
	{
		Element *temp = current;
		current = current->next;
		free(temp->nameElement);
		free(temp->nameType);
		free(temp);
	}
}

/**
 * @param list: element list pointer
 * @param nameElement: Name of the element to search for
 * @brief Searches for an element with the given name in the list
 * @return Pointer to the found element or NULL if not found
 */
Element *searchElementName(const Element *list, const char *nameElement)
{
	if (list == NULL || nameElement == NULL)
	{
		return NULL;
	}

	const Element *current = list;
	while (current != NULL)
	{
		if (strcmp(current->nameElement, nameElement) == 0)
		{
			return (Element *)current;
		}
		current = current->next;
	}
	return NULL;
}

/**
 * @param list: element list pointer
 * @param index: Index of the element to search for
 * @brief Searches for an element at the given index in the list
 * @return Pointer to the found element or NULL if not found
 */
Element *searchElementIndex(const Element *list, const int index)
{
	if (list == NULL || index < 0)
	{
		return NULL;
	}

	for (int i = 0; i < index && list != NULL; i++)
	{
		list = list->next;
	}
	return (Element *)list;
}

/**
 * @param list: element list pointer
 * @brief Draws a random element from the list
 * @return Pointer to the drawn element or NULL if the list is empty
 */
Element *drawRandomElement(const Element *list)
{
	if (list == NULL)
	{
		return NULL;
	}

	int count = 0;
	const Element *current = list;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}

	if (count == 0)
	{
		return NULL;
	}

	int randomIndex = rand() % count;
	return searchElementIndex(list, randomIndex);
}

/**
 * @brief fetches a list of Element from a file (if filepath is NULL from a hardcoded list)
 * @param filepath: the file path (either relative or absolute), set to NULL to get the default Element list
 * @param eventType: the event type, accepted valued are ["Event", "Action"], if no value is specified, or is invalid, will return Event types.
 * @return a list of Element parsed from said file.
 */
Element *fetchEventsFromFile(const char *filepath, char *eventType)
{
	if (filepath == NULL)
	{
		// If Action is specified
		if (strcmp(eventType, "Action") == 0)
		{
			return fetchDefaultActionList();
		}
		else if (strcmp(eventType, "Event") == 0) // if Event is specified
		{
			return fetchDefaultEventList();
		}
		else
		{
			fprintf(stderr, "fetchEventsFromFile: Wrong eventType: %s was specified but [\"Event\", \"Action\"] were expected.\nReturned Elements will be of type \"Event\".", eventType);
			return fetchDefaultEventList();
		}
	}

	FILE *f = fopen(filepath, "r");
	if (f == NULL)
	{
		fprintf(stderr, "[ERROR] fetchElementsFromFile: Could not open file");
		return NULL;
	}

	// Get filesize
	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	// malloc
	char *json_data = malloc(fileSize + 1);

	// fill buffer
	fread(json_data, 1, fileSize, f);
	json_data[fileSize] = '\0';
	fclose(f);

	// Parsing the json
	cJSON *root = cJSON_Parse(json_data);
	free(json_data);

	if (root == NULL)
	{
		fprintf(stderr, "[ERROR] fetchElementsFromFile: Could not parse JSON correctly");
		return NULL;
	}

	Element *eltList = NULL;

	char targetKey[32];
	snprintf(targetKey, sizeof(targetKey), "%ss", eventType);

	cJSON *items = cJSON_GetObjectItemCaseSensitive(root, targetKey);
	if (!cJSON_IsArray(items))
	{
		fprintf(stderr, "[ERROR] fetchElementsFromFile: Could not find valid array for %s in JSON.\n", targetKey);
		cJSON_Delete(root);
		return NULL;
	}

	cJSON *item = NULL;
	cJSON_ArrayForEach(item, items)
	{
		cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
		cJSON *type = cJSON_GetObjectItemCaseSensitive(item, "type");
		cJSON *change_type_str = cJSON_GetObjectItemCaseSensitive(item, "change_type");
		cJSON *target_field = cJSON_GetObjectItemCaseSensitive(item, "target_field");
		cJSON *value = cJSON_GetObjectItemCaseSensitive(item, "value");

		if (cJSON_IsString(name) && cJSON_IsString(type) && cJSON_IsNumber(value))
		{
			int change_enum = parseChangeType(change_type_str->valuestring);

			ElementChangeValue change_val = {0};
			if (strcmp(target_field->valuestring, "quantity") == 0)
			{
				change_val.quantity = value->valueint;
			}
			else if (strcmp(target_field->valuestring, "population") == 0)
			{
				change_val.population = value->valueint;
			}

			addElement(&eltList, name->valuestring, type->valuestring, change_enum, change_val);
		}
	}

	cJSON_Delete(root);

	return eltList;
}

/**
 * @brief Parses the ElementChangeTarget from a string
 */
int parseChangeType(const char *typeStr)
{
	if (strcmp(typeStr, "ELEMENT_CHANGE_WATER") == 0)
		return ELEMENT_CHANGE_WATER;
	if (strcmp(typeStr, "ELEMENT_CHANGE_WOOD") == 0)
		return ELEMENT_CHANGE_WOOD;
	if (strcmp(typeStr, "ELEMENT_CHANGE_MINERALS") == 0)
		return ELEMENT_CHANGE_MINERALS;
	if (strcmp(typeStr, "ELEMENT_CHANGE_HABITABILITY") == 0)
		return ELEMENT_CHANGE_HABITABILITY;
	if (strcmp(typeStr, "ELEMENT_CHANGE_POPULATION") == 0)
		return ELEMENT_CHANGE_POPULATION;
	return 0; // Default (NONE)
}

// *****************************************************************
// *****************************************************************
// *** Default Events **********************************************
// *****************************************************************
// *****************************************************************

// The following events were generated by ChatGpt for time gain,
// a future revision is possible.

Element *fetchDefaultEventList()
{
	Element *climaticEvents = NULL;

	addElement(&climaticEvents, "Heatwave", "Event", ELEMENT_CHANGE_WATER, (ElementChangeValue){.quantity = -500});

	addElement(&climaticEvents, "Drought", "Event", ELEMENT_CHANGE_WATER, (ElementChangeValue){.quantity = -1000});

	addElement(&climaticEvents, "HeavyRainfall", "Event", ELEMENT_CHANGE_WATER, (ElementChangeValue){.quantity = 800});

	addElement(&climaticEvents, "VolcanicEruption", "Event", ELEMENT_CHANGE_MINERALS, (ElementChangeValue){.quantity = 300});

	addElement(&climaticEvents, "ForestFire", "Event", ELEMENT_CHANGE_WOOD, (ElementChangeValue){.quantity = -400});

	addElement(&climaticEvents, "ColdWave", "Event", ELEMENT_CHANGE_HABITABILITY, (ElementChangeValue){.quantity = -20});

	addElement(&climaticEvents, "Flood", "Event", ELEMENT_CHANGE_POPULATION, (ElementChangeValue){.population = -500});

	addElement(&climaticEvents, "SeaLevelRise", "Event", ELEMENT_CHANGE_HABITABILITY, (ElementChangeValue){.quantity = -10});

	return climaticEvents;
}

Element *fetchDefaultActionList()
{
	Element *climaticActions = NULL;

	addElement(&climaticActions, "Reforestation", "Action", ELEMENT_CHANGE_WOOD, (ElementChangeValue){.quantity = 500});

	addElement(&climaticActions, "Irrigation", "Action", ELEMENT_CHANGE_WATER, (ElementChangeValue){.quantity = -200});

	addElement(&climaticActions, "Desalination", "Action", ELEMENT_CHANGE_WATER, (ElementChangeValue){.quantity = 400});

	addElement(&climaticActions, "MineralExtraction", "Action", ELEMENT_CHANGE_MINERALS, (ElementChangeValue){.quantity = -150});

	addElement(&climaticActions, "EmissionReduction", "Action", ELEMENT_CHANGE_HABITABILITY, (ElementChangeValue){.quantity = 10});

	addElement(&climaticActions, "UrbanExpansion", "Action", ELEMENT_CHANGE_POPULATION, (ElementChangeValue){.population = 1000});

	addElement(&climaticActions, "ClimateAdaptation", "Action", ELEMENT_CHANGE_HABITABILITY, (ElementChangeValue){.quantity = 15});

	addElement(&climaticActions, "LandRestoration", "Action", ELEMENT_CHANGE_WOOD, (ElementChangeValue){.quantity = 250});

	return climaticActions;
}