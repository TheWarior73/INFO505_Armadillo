#include "element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @param list: element list pointer
 * @param nameElement: Name of the element to add
 * @param nameType: Type of the element to add
 * @brief Adds an element to the beginning of the list
 */
void addElement(Element **list, const char *nameElement, const char *nameType)
{
	if (list == NULL || nameElement == NULL || nameType == NULL) {
		return;
	}

	Element *nouvelElement = malloc(sizeof(*nouvelElement));
	if (nouvelElement == NULL) {
		return;
	}

	size_t tailleNomElement = strlen(nameElement) + 1;
	size_t tailleNomType = strlen(nameType) + 1;
	nouvelElement->nameElement = malloc(tailleNomElement);
	nouvelElement->nameType = malloc(tailleNomType);

	// Malloc fail
	if (nouvelElement->nameElement == NULL || nouvelElement->nameType == NULL)
	{
		free(nouvelElement->nameElement);
		free(nouvelElement->nameType);
		free(nouvelElement);
		return;
	}

	memcpy(nouvelElement->nameElement, nameElement, tailleNomElement);
	memcpy(nouvelElement->nameType, nameType, tailleNomType);
	nouvelElement->next = *list;
	*list = nouvelElement;
}

/*
 * @param list: element list pointer
 * @param nameElement: Name of the element to add
 * @param nameType: Type of the element to add
 * @brief Adds an element to the end of the list
 */
void addElementTail(Element **list, const char *nameElement, const char *nameType)
{
    if (list == NULL || nameElement == NULL || nameType == NULL) {
		return;
	}

    Element *nouvelElement = malloc(sizeof(*nouvelElement));
	if (nouvelElement == NULL) {
		return;
	}

	size_t tailleNomElement = strlen(nameElement) + 1;
	size_t tailleNomType = strlen(nameType) + 1;
	nouvelElement->nameElement = malloc(tailleNomElement);
	nouvelElement->nameType = malloc(tailleNomType);

	if (nouvelElement->nameElement == NULL || nouvelElement->nameType == NULL)
	{
		free(nouvelElement->nameElement);
		free(nouvelElement->nameType);
		free(nouvelElement);
		return;
	}

    memcpy(nouvelElement->nameElement, nameElement, tailleNomElement);
    memcpy(nouvelElement->nameType, nameType, tailleNomType);
    nouvelElement->next = NULL;

    if (*list == NULL) {
        *list = nouvelElement;
        return;
    }

    Element *current = *list;
    while (current->next != NULL) {
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
		printf("Element name: %s, Type: %s\n", current->nameElement, current->nameType);
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

/*
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

/*
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