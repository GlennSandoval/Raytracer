#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <stdlib.h>
#include "Color.h"

const float 	PI 			= 3.1415926535897932384f;
const float 	TWO_PI 		= 6.2831853071795864769f;
const float 	PI_ON_180 	= 0.0174532925199432957f;
const float 	invPI 		= 0.3183098861837906715f;
const float 	invTWO_PI 	= 0.1591549430918953358f;
const float     LARGE_VAL   = 1e10f;

const Color black(0.0);
const Color white(1.0);
const Color red(1.0, 0.0, 0.0);
const Color green(0.0, 1.0, 0.0);
const Color blue(0.0, 0.0, 1.0);
const Color yellow(1.0, 1.0, 0.0);
const Color cyan(0.0, 1.0, 1.0);
const Color magenta(1.0, 0.0, 1.0);
const Color orange(0.5, 0.5, 0.0);

#endif