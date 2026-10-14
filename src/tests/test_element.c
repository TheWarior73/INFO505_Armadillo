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
	addElement(&list, "second", "type-b");
	addElement(&list, "first", "type-a");

	assert_element(list, "first", "type-a");
	assert_element(list->next, "second", "type-b");
	assert(list->next->next == NULL);

	addElement(NULL, "ignored", "type");
	addElement(&list, NULL, "type");
	assert_element(list, "first", "type-a");

	displayElements(list);

	freeListElements(list);
}

static void test_add_element_at_tail(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a");
	assert_element(list, "first", "type-a");
	assert(list->next == NULL);

	addElementTail(&list, "second", "type-b");
	assert_element(list->next, "second", "type-b");
	assert(list->next->next == NULL);

	addElementTail(NULL, "ignored", "type");
	addElementTail(&list, "ignored", NULL);
	assert(list->next->next == NULL);

	freeListElements(list);
}

static void test_delete_element(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a");
	addElementTail(&list, "middle", "type-b");
	addElementTail(&list, "last", "type-c");

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
}

static void test_search_elements(void)
{
	Element *list = NULL;
	addElementTail(&list, "first", "type-a");
	addElementTail(&list, "second", "type-b");

	assert(searchElementName(list, "second") == list->next);
	assert(searchElementName(list, "missing") == NULL);
	assert(searchElementName(list, NULL) == NULL);
	assert(searchElementName(NULL, "first") == NULL);

	assert(searchElementIndex(list, 0) == list);
	assert(searchElementIndex(list, 1) == list->next);
	assert(searchElementIndex(list, 2) == NULL);
	assert(searchElementIndex(list, -1) == NULL);
	assert(searchElementIndex(NULL, 0) == NULL);

	freeListElements(list);
}

static void test_draw_random_element(void)
{
	Element *list = NULL;
	assert(drawRandomElement(NULL) == NULL);

	addElementTail(&list, "first", "type-a");
	addElementTail(&list, "second", "type-b");
	addElementTail(&list, "third", "type-c");

	for (int i = 0; i < 100; i++)
	{
		Element *drawn = drawRandomElement(list);
		assert(drawn == list || drawn == list->next || drawn == list->next->next);
	}

	freeListElements(list);
}

static void test_display_elements(void)
{
	Element *list = NULL;
	displayElements(list);
	addElement(&list, "sample", "type");
	displayElements(list);
	freeListElements(list);
	freeListElements(NULL);
}

int main(void)
{
	test_add_element_at_head();
	test_add_element_at_tail();
	test_delete_element();
	test_search_elements();
	test_draw_random_element();
	test_display_elements();
	puts("All Element tests passed.");
	return 0;
}