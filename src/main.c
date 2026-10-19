#include "../libs/core/Element/element.c"
#include <stdlib.h>

#include "tests/test_element.c"
#include "tests/test_planet.c"

int main(void)
{

    test_liste_chainee();
    test_planet();
	return EXIT_SUCCESS;
}