#include "GlossyReflective.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

GlossyReflective::GlossyReflective() : reflectivity(1.0)
{}

Color GlossyReflective::shade(ShadeRec& sr) {
	Color col = Phong::shade(sr);
	Vector3D v = -sr.ray.d;
	Vector3D r = sr.ray.d + 2.0f * (sr.nh * v) * sr.nh;

	float floor = -1.0f, ceiling = 1.0f, range = (ceiling - floor);
	float rnd1 = floor + (range * rand()) / (RAND_MAX + 1.0f);
	float rnd2 = floor + (range * rand()) / (RAND_MAX + 1.0f);
	float rnd3 = floor + (range * rand()) / (RAND_MAX + 1.0f);

	Point3D rand = Point3D(rnd1, rnd2, rnd3);
	rand = rand * 0.1f;
	r = r + rand;
	r.normalize();
	Ray reflectedRay = Ray(sr.ph+0.01f*r, r);

	col += reflectivity * sr.world.trace_ray(reflectedRay, sr.depth + 1);

	return col;
}