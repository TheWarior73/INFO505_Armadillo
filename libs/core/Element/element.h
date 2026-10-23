#ifndef ELEMENT_H
#define ELEMENT_H

typedef enum
{
	ELEMENT_CHANGE_NONE,
	ELEMENT_CHANGE_POPULATION,
	ELEMENT_CHANGE_RADIUS,
	ELEMENT_CHANGE_WATER,
	ELEMENT_CHANGE_MINERALS,
	ELEMENT_CHANGE_WOOD,
	ELEMENT_CHANGE_HABITABILITY
} ElementChangeTarget;

typedef union
{
	long population;
	double radiusKm;
	int quantity;
} ElementChangeValue;

typedef struct Element
{
	char *nameElement;
	char *nameType;
	ElementChangeTarget changeTarget;
	ElementChangeValue change;
	
	struct Element *next;
} Element;


Element *createElement(const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change);
void addElement(Element **list, const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change);
void addElementTail(Element **list, const char *nameElement, const char *nameType, ElementChangeTarget target, ElementChangeValue change);
void deleteElement(Element **liste, const char *nameElement);
void displayElements(const Element *liste);
void freeListElements(Element *liste);
Element *searchElementName(const Element *liste, const char *nameElement);

Element *searchElementIndex(const Element *list, const int index);
Element *drawRandomElement(const Element *liste);

#endif