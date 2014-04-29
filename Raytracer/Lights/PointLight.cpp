#include "PointLight.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

PointLight::PointLight() : ls(100.0), color(white), location(Point3D(0,0,0))
{}

Vector3D PointLight::get_direction(const ShadeRec& sr) {
	return (location - sr.ph).normalized();
}

Point3D PointLight::get_position() const {
	return location;
}

Color PointLight::L(const ShadeRec& sr) {
	float d = (sr.ph - location).mag();
	
	if (casts_shadows()) {
		Vector3D r = (location - sr.ph);
		r.normalize();
		Ray shRay = Ray(sr.ph+0.01f*r, r);
		float t;
		float Li = 0.0;
		for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
			if (sr.world.get_object(i)->shadow_hit(shRay, t, Li) && t < d) {
				float csh = 1 - (Li/d);
				if (csh < 0) csh = 0.0;
				return (ls * color) * csh / (2.5f * d);
			}
		}
	}
	return (ls * color) / (1.5f * d);
}

bool PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) {
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