#include "AreaLight.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

AreaLight::AreaLight() : Light(), object_ptr(NULL), material_ptr(NULL)
{}

Vector3D AreaLight::get_direction(const ShadeRec& sr) {
	sample_point = object_ptr->sample();
	normal = object_ptr->get_normal(sample_point);
	wi = (sample_point - sr.ph).normalized();
	return wi;
}

Color AreaLight::L(const ShadeRec& sr) {
	
	float t;
	float Li = 0.0;
	Color light_color = material_ptr->get_Le(sr);

	Point3D sp = object_ptr->sample(); // sample a point on the area light
	Vector3D nlh = (sp - sr.ph).normalized(); // get the direction from the sample point to the hit point
	Ray ray = Ray(sr.ph, nlh); // construct a shadow ray from hit point toward light
	float d = (sp - ray.o).mag(); // distance from sample point to ray origin

	for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
		if (sr.world.get_object(i)->shadow_hit(ray, t, Li) && t < d) {
			return black / (float)(sr.world.get_sampler()->get_num_samples());
		}
	}
	return light_color / (float)(sr.world.get_sampler()->get_num_samples());
}

bool AreaLight::in_shadow(const Ray& ray, const ShadeRec& sr) {
	float t;
	float d = (sample_point - ray.o) * ray.d;
	float Li = 0.0;

	for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
		if (sr.world.get_object(i)->shadow_hit(ray, t, Li) && t < d) {
			return true;
		}
	}
	return false;
}