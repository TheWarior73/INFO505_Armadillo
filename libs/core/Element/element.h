#ifndef ELEMENT_H
#define ELEMENT_H

typedef struct Element
{
	char *nameElement;
	char *nameType;
	struct Element *next;
} Element;

// TODO: create Element (if we ever have to create new elements in other functions)
// currently it is a duplicated line of code in addElement & addElementTail

void addElement(Element **list, const char *nameElement, const char *nameType);
void addElementTail(Element **liste, const char *nameElement, const char *nameType);
void deleteElement(Element **liste, const char *nameElement);
void displayElements(const Element *liste);
void freeListElements(Element *liste);
Element *searchElementName(const Element *liste, const char *nameElement);

Element *searchElementIndex(const Element *list, const int index);
Element *drawRandomElement(const Element *liste);

#endif