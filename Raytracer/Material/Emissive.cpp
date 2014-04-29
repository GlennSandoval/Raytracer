#include "Emissive.h"
#include "../Utilities/Constants.h"

Emissive::Emissive() : radiance(1.0), color(white)
{}

Color Emissive::shade(ShadeRec& sr) {
	if (-sr.nh * sr.ray.d > 0.0)
		return (radiance * color);
	else
		return black;
}