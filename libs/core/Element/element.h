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

/**
 * @brief Element display Flags used in the display Element
 */
typedef enum
{
	ELEMENT_DISPLAY_MINIMAL,			  // Format -> ElementName
	ELEMENT_DISPLAY_NAME_ONLY,			  // Format -> Name: ElementName
	ELEMENT_DISPLAY_NAME_TYPE,			  // Format -> Name: ElementName	Type: ElementType
	ELEMENT_DISPLAY_NAME_IMPACT_QUANTITY, // Format -> Name: ElementName	(+ | -)Quantity ElementChangeTarget
	ELEMENT_DISPLAY_ALL,				  // Format -> Name: ElementName	(-)?Quantity ElementChangeTarget		Type: ElementType
} ElementDisplayFlags;

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
void displayElements(const Element *liste, ElementDisplayFlags flags);
void freeListElements(Element *liste);
Element *searchElementName(const Element *liste, const char *nameElement);

char *strOfElementChangeTarget(ElementChangeTarget target);

Element *searchElementIndex(const Element *list, const int index);
Element *drawRandomElement(const Element *liste);

Element *fetchEventsFromFile(const char *filepath, char *eventType);
int parseChangeType(const char *typeStr);
Element *fetchDefaultEventList();
Element *fetchDefaultActionList();

#endif