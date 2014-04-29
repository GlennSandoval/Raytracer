#include "Reflective.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Reflective::Reflective() : reflectivity(1.0)
{}

Color Reflective::shade(ShadeRec& sr) {
	Color col = Phong::shade(sr);
	Vector3D v = -sr.ray.d;
	Vector3D r = sr.ray.d + 2.0f * (sr.nh * v) * sr.nh;
	Ray reflectedRay = Ray(sr.ph+0.01f*r, r);

	col += reflectivity * sr.world.trace_ray(reflectedRay, sr.depth + 1);

	return col;
}
