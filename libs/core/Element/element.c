#include "element.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @param liste: Pointeur vers la liste des éléments
 * @param nomElement: Nom de l'élément à ajouter
 * @param nomType: Type de l'élément à ajouter
 */
void addElement(Element **liste, const char *nameElement, const char *nameType)
{
	if (liste == NULL || nameElement == NULL || nameType == NULL) { //si pointeurs nuls
		return;
	}

	Element *nouvelElement = malloc(sizeof(*nouvelElement)); //allocation pour nouvel élément
	if (nouvelElement == NULL) {
		return;
	}

	size_t tailleNomElement = strlen(nameElement) + 1;
	size_t tailleNomType = strlen(nameType) + 1;
	nouvelElement->nameElement = malloc(tailleNomElement);
	nouvelElement->nameType = malloc(tailleNomType);

	if (nouvelElement->nameElement == NULL || nouvelElement->nameType == NULL) {
		free(nouvelElement->nameElement);
		free(nouvelElement->nameType);
		free(nouvelElement);
		return;
	}

	memcpy(nouvelElement->nameElement, nameElement, tailleNomElement);
	memcpy(nouvelElement->nameType, nameType, tailleNomType);
	nouvelElement->next = *liste;
	*liste = nouvelElement;
}


void addElementTail(Element **liste, const char *nameElement, const char *nameType)
{
    if (liste == NULL || nameElement == NULL || nameType == NULL) { //if null pointer
		return;
	}

	Element *nouvelElement = malloc(sizeof(*nouvelElement)); //new element allocation
	if (nouvelElement == NULL) {
		return;
	}

    size_t tailleNomElement = strlen(nameElement) + 1;
	size_t tailleNomType = strlen(nameType) + 1;
	nouvelElement->nameElement = malloc(tailleNomElement);
	nouvelElement->nameType = malloc(tailleNomType);

	if (nouvelElement->nameElement == NULL || nouvelElement->nameType == NULL) {
		free(nouvelElement->nameElement);
		free(nouvelElement->nameType);
		free(nouvelElement);
		return;
	}

    for (Element *current = *liste; current != NULL; current = current->next) {
        if (current->next == NULL) {
            current->next = nouvelElement;
            break;
        }
    }
}

void deleteElement(Element **liste, const char *nameElement)
{
    if (liste == NULL || *liste == NULL || nameElement == NULL) {
        return;
    }

    Element *current = *liste;
    Element *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->nameElement, nameElement) == 0) {
            if (previous == NULL) {
                *liste = current->next; //first element case
            } else {
                previous->next = current->next; //other elements case
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

void displayElements(const Element *liste)
{
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    const Element *current = liste;
    while (current != NULL) {
        printf("Nom de l'élément: %s, Type: %s\n", current->nameElement, current->nameType);
        current = current->next;
    }
}

void freeListElements(Element *list)
{
    Element *current = list;
    while (current != NULL) {
        Element *temp = current;
        current = current->next;
        free(temp->nameElement);
        free(temp->nameType);
        free(temp);
    }
}

Element* searchElementName(const Element *liste, const char *nameElement)
{
    if (liste == NULL || nameElement == NULL) {
        return NULL;
    }

    const Element *current = liste;
    while (current != NULL) {
        if (strcmp(current->nameElement, nameElement) == 0) {
            return (Element *)current;
        }
        current = current->next;
    }
    return NULL;
}

Element *searchElementIndex(const Element *list, const int index){
    if (list == NULL || index < 0) {
        return NULL;
    }

    for (int i = 0; i < index && list != NULL; i++) {
        list = list->next;
    }
    return (Element *)list;
}

Element *drawRandomElement(const Element *liste)
{
    if (liste == NULL) {
        return NULL;
    }

    int count = 0;
    const Element *current = liste;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count == 0) {
        return NULL;
    }

    int randomIndex = rand() % count;
    return searchElementIndex(liste, randomIndex);
}