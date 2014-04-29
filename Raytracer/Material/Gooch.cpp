#include "Gooch.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Gooch::Gooch()
{}

Color Gooch::shade(ShadeRec& sr) {
	Color col = black;
	float c = 0.0;
	
	for (int i = 0; i < sr.world.get_num_lights(); i++) {
		Vector3D nlh = sr.world.get_light(i)->get_direction(sr);
		float cosTheta = sr.nh * nlh;

		c = (cosTheta + 1) / 2;

		float alpha = 0.1f;

		col += (warm_color * c) * (1 - alpha) + alpha * cool_color;
	}
	return col;
}