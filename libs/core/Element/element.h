#ifndef ELEMENT_H
#define ELEMENT_H

/**
 * @brief The Target of the element quantity modification
 */
typedef enum
{

	/** @brief No change */
	ELEMENT_CHANGE_NONE,

	/** @brief Population Modification */
	ELEMENT_CHANGE_POPULATION,

	/** @brief Radius Modification */
	ELEMENT_CHANGE_RADIUS,

	/** @brief Water Modification */
	ELEMENT_CHANGE_WATER,

	/** @brief Minerals Modification */
	ELEMENT_CHANGE_MINERALS,

	/** @brief Wood Modification */
	ELEMENT_CHANGE_WOOD,

	/** @brief Habitability Modification */
	ELEMENT_CHANGE_HABITABILITY
} ElementChangeTarget;

/**
 * @brief Element display Flags used in the display Element
 */
typedef enum
{

	/** @brief Format -> ElementName */
	ELEMENT_DISPLAY_MINIMAL,

	/** @brief Format -> Name: ElementName */
	ELEMENT_DISPLAY_NAME_ONLY,

	/** @brief Format -> Name: ElementName	Type: ElementType */
	ELEMENT_DISPLAY_NAME_TYPE,

	/** @brief Format -> Name: ElementName	(+ | -)Quantity ElementChangeTarget */
	ELEMENT_DISPLAY_NAME_IMPACT_QUANTITY,

	/** @brief Format -> Name: ElementName	(-)?Quantity ElementChangeTarget		Type: ElementType */
	ELEMENT_DISPLAY_ALL,
} ElementDisplayFlags;

/**
 * @brief Holds a change value depending on the element change target
 */
typedef union
{
	long population;
	double radiusKm;
	int quantity;
} ElementChangeValue;

/**
 * @brief Represents an Element
 */
typedef struct Element
{
	/** the name of the Element*/
	char *nameElement;

	/** The Type of the Element accepted values are [Action, Event] */
	char *nameType;

	/** The change Target of the Element, what value will be affected by the Element if used (ex: population, habitability, ...) */
	ElementChangeTarget changeTarget;

	/** The change value applied to the value field Targeted (ex: wood, water, ...) */
	ElementChangeValue change;

	/** the next Element in the list */
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