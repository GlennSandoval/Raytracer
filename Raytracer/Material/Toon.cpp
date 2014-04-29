#include "Toon.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Toon::Toon()
{}

Color Toon::shade(ShadeRec& sr) {
	Color col = black;
	float c = 0.0;

	for (int i = 0; i < sr.world.get_num_lights(); i++) {
		Vector3D nlh = sr.world.get_light(i)->get_direction(sr);
		float cosTheta = sr.nh * nlh;

		c = (cosTheta + 1) / 2;

		if (c < 0) c = 0;
		if (c > 1) c = 1;

		col += (warm_color * c) + (1 - c) * cool_color;
	}
	return col;
}