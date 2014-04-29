#include "Spotlight.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

Spotlight::Spotlight() : ls(1.0f), color(white), location(Point3D(1,10,10)), alpha(0.2f * PI), alpha2(0.45f * PI), tex_ptr(NULL)
{
	direction = (Point3D(0,0,0) - location).normalized();
}

Vector3D Spotlight::get_direction(const ShadeRec& sr) {
	return (location - sr.ph).normalized();
}

Point3D Spotlight::get_position() const {
	return location;
}

Color Spotlight::L(const ShadeRec& sr) {
	Vector3D nlh = (location - sr.ph).normalized();
	float a = (-nlh * direction);
	if (a < cos(alpha)) a = 0;
	else a = a * cos(alpha2);
	float c = (sr.nh * nlh) * a;

	if (tex_ptr) {
		float x = sr.world.get_camera().u * (sr.ph - sr.world.get_camera().p0) / 6.0f;
		float y = sr.world.get_camera().v * (sr.ph - sr.world.get_camera().p0) / 6.0f;
		if (0 <= x && x <= 1 && 0 <= y && y <= 1) {
			Color proj = tex_ptr->get_color(sr);
			return ls * proj * c;
		}
	}

	return ls * color * c;
}

bool Spotlight::in_shadow(const Ray& ray, const ShadeRec& sr) {
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