#include "SphericalMapping.h"
#include "../Utilities/Constants.h"
#include <math.h>

void SphericalMapping::get_pixel_coordinates(const ShadeRec& sr, const int hres, const int vres, int& row, int& col) {
	float phi = acos(sr.local_ph.y);
	float theta = acos(sr.local_ph.x / sin(phi));

	if (phi < 0) phi = TWO_PI - phi;

	float u = theta * invTWO_PI;
	float v = 1 - phi * invPI;

	if (u < 0) u = 1 - u;
	if (v < 0) v = 1 - v;

	col = (int) (hres - 1) * u;
	row = (int) (vres - 1) * v;
}
