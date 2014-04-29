#include "DirectionalLight.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

DirectionalLight::DirectionalLight() : ls(1.0), color(white), location(Point3D(10000,10000,-10000)), direction(Vector3D(-1,-1,0))
{}

Vector3D DirectionalLight::get_direction(const ShadeRec& sr) {
	return (-direction).normalized();
}

Point3D DirectionalLight::get_position() const {
	return location;
}

Color DirectionalLight::L(const ShadeRec& sr) {
	return ls * color;
}

bool DirectionalLight::in_shadow(const Ray& ray, const ShadeRec& sr) {
	float t;
	float d = (location - ray.o).mag();
	float Li = 0.0;

	for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
		if (sr.world.get_object(i)->shadow_hit(ray, t, Li) && t < d) {
			return true;
		}
	}
	return false;
}