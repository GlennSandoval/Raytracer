#include "Refractive.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Refractive::Refractive() : eta(1.0), transmission(1.0), refraction_ptr(NULL)
{}

Color Refractive::shade(ShadeRec& sr) {
	Color col = Phong::shade(sr);

	// get the texture
	if (refraction_ptr) {
		eta = 2.0f * refraction_ptr->get_color(sr).r;
	}

	Vector3D v = -sr.ray.d.normalized();
	Vector3D n = sr.nh;

	float cosTheta1 = sr.nh * v;
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

	Ray transmittedRay = Ray(sr.ph+0.01f*transmitDir, transmitDir);

	col += sr.world.trace_ray(transmittedRay, sr.depth + 1);

	return col;
}