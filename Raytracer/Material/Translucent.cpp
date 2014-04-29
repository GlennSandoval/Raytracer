#include "Translucent.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Translucent::Translucent() : eta(1.0)
{}

Color Translucent::shade(ShadeRec& sr) {
	Color col = Phong::shade(sr);

	Vector3D v = -sr.ray.d.normalized();
	Vector3D n = sr.nh;

	float cosTheta1 = n * v;
	float theeta = eta;
	if(cosTheta1 < 0)
	{
		n = -n;
		cosTheta1 = -cosTheta1;
		theeta = 1.0f / theeta;
	}

	float a = -1.0f / theeta;
	float term = ((cosTheta1 * cosTheta1 -1) / (theeta * theeta)) + 1;
	
	Vector3D transmitDir;
	if (term > 0.0) {
		float b = (cosTheta1 / theeta) - sqrt(term);
		transmitDir = a * v + b * n;
		transmitDir.normalize();
	}
	else {
		transmitDir = -v + 2.0f * cosTheta1 * n;
		transmitDir.normalize();
	}

	float floor = -1.0, ceiling = 1.0, range = (ceiling - floor);
	float rnd1 = floor + (range * rand()) / (RAND_MAX + 1.0f);
	float rnd2 = floor + (range * rand()) / (RAND_MAX + 1.0f);
	float rnd3 = floor + (range * rand()) / (RAND_MAX + 1.0f);

	Point3D rand = Point3D(rnd1, rnd2, rnd3);
	rand = rand * 0.1f;
	transmitDir = transmitDir + rand;
	transmitDir.normalize();

	Ray transmittedRay = Ray(sr.ph+0.01f*transmitDir, transmitDir);

	col += sr.world.trace_ray(transmittedRay, sr.depth + 1);

	return col;
}
