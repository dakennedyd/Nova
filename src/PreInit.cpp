#include "PreInit.h"

/*
* Checks for IEEE754 standard compliance
*/
static_assert(sizeof(float) == 4, "Assertion Failed: Incorrect size of float");
static_assert(sizeof(double) == 8, "Assertion Failed: Incorrect size of double");

