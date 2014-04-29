#include "AmbientOcclusion.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

AmbientOcclusion::AmbientOcclusion() : Light(), ls(6.0), color(white)
{
	geodesic_ptr = new Geodesic("SampleAssets/objs/642.obj");
}

Vector3D AmbientOcclusion::get_direction(const ShadeRec& sr) {
	u = sr.world.get_camera().u;
	v = sr.world.get_camera().v;
	w = sr.world.get_camera().w;

	sample_point = geodesic_ptr->get_sample(); //sr.world.sampler_ptr->sample_hemisphere();
	return (sample_point.x * u + sample_point.y * v + sample_point.z * w);
}

Color AmbientOcclusion::L(const ShadeRec& sr) {
	float Li = 0.0;

	Ray ray = Ray(sr.ph, get_direction(sr)); // construct a shadow ray from hit point toward light

	if (in_shadow(ray, sr))
		return black;
	return ls * color; // / (sr.world.sampler_ptr->get_num_samples() * sr.world.sampler_ptr->get_num_samples());
}

bool AmbientOcclusion::in_shadow(const Ray& ray, const ShadeRec& sr) {
	float t;
	float d = (sample_point - ray.o) * ray.d;
	float Li = 0.0;

	for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
		if (sr.world.get_object(i)->shadow_hit(ray, t, Li)) {
			return true;
		}
	}
	return false;
}