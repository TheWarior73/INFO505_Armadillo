#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../libs/core/Element/element.h"

static void assert_element(const Element *element, const char *name, const char *type)
{
	assert(element != NULL);
	assert(strcmp(element->nameElement, name) == 0);
	assert(strcmp(element->nameType, type) == 0);
}

static void test_add_element_at_head(void)
{
	Element *list = NULL;
	addElement(&list, "second", "type-b", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElement(&list, "first", "type-a", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});

	assert_element(list, "first", "type-a");
	assert_element(list->next, "second", "type-b");
	assert(list->next->next == NULL);

	addElement(NULL, "ignored", "type", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElement(&list, NULL, "type", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	assert_element(list, "first", "type-a");

	displayElements(list);

	freeListElements(list);
}

static void test_add_element_at_tail(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	assert_element(list, "first", "type-a");
	assert(list->next == NULL);

	addElementTail(&list, "second", "type-b", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	assert_element(list->next, "second", "type-b");
	assert(list->next->next == NULL);

	addElementTail(NULL, "ignored", "type", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "ignored", NULL, ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	assert(list->next->next == NULL);

	displayElements(list);
	freeListElements(list);
}

static void test_add_element_with_planet_change(void)
{
	Element *list = NULL;
	ElementChangeValue change = {.quantity = -5};

	addElement(&list, "Drought", "event", ELEMENT_CHANGE_WATER, change);
	assert_element(list, "Drought", "event");
	assert(list->changeTarget == ELEMENT_CHANGE_WATER);
	assert(list->change.quantity == -5);

	addElement(&list, "No effect", "action", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	assert(list->changeTarget == ELEMENT_CHANGE_NONE);
	assert(list->change.population == 0);

	freeListElements(list);
}

static void test_delete_element(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "middle", "type-b", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "last", "type-c", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});

	displayElements(list);

	deleteElement(&list, "middle");
	assert_element(list, "first", "type-a");
	assert_element(list->next, "last", "type-c");

	deleteElement(&list, "missing");
	deleteElement(&list, NULL);
	assert_element(list, "first", "type-a");

	deleteElement(&list, "first");
	assert_element(list, "last", "type-c");
	deleteElement(&list, "last");
	assert(list == NULL);
	deleteElement(&list, "missing");
	deleteElement(NULL, "missing");

	displayElements(list);
}

static void test_search_elements(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "second", "type-b", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});

	assert(searchElementName(list, "second") == list->next);
	assert(searchElementName(list, "missing") == NULL);
	assert(searchElementName(list, NULL) == NULL);
	assert(searchElementName(NULL, "first") == NULL);

	assert(searchElementIndex(list, 0) == list);
	assert(searchElementIndex(list, 1) == list->next);
	assert(searchElementIndex(list, 2) == NULL);
	assert(searchElementIndex(list, -1) == NULL);
	assert(searchElementIndex(NULL, 0) == NULL);

	displayElements(list);
	freeListElements(list);
}

static void test_draw_random_element(void)
{
	Element *list = NULL;
	assert(drawRandomElement(NULL) == NULL);

	addElementTail(&list, "first", "type-a", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "second", "type-b", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	addElementTail(&list, "third", "type-c", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});

	for (int i = 0; i < 100; i++)
	{
		Element *drawn = drawRandomElement(list);
		assert(drawn == list || drawn == list->next || drawn == list->next->next);
	}

	displayElements(list);
	freeListElements(list);
}

static void test_display_elements(void)
{
	Element *list = NULL;
	displayElements(list);
	addElement(&list, "sample", "type", ELEMENT_CHANGE_NONE, (ElementChangeValue){0});
	displayElements(list);
	freeListElements(list);
	freeListElements(NULL);
}

int test_liste_chainee(void)
{
	test_add_element_at_head();
	test_add_element_at_tail();
	test_add_element_with_planet_change();
	test_delete_element();
	test_search_elements();
	test_draw_random_element();
	test_display_elements();
	puts("All Element tests passed.");
	return 0;
}