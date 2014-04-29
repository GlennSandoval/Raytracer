#include "Plane.h"

Plane::Plane() : point(Point3D(0,0,0)), normal(Normal(0,1,0))
{}

Plane::Plane(const Point3D p, const Normal& n) : point(p), normal(n)
{}

bool Plane::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	float temp = (normal * (point - ray.o)) / (ray.d * normal);

	if (temp > 0 && temp < tmin) {
		t = temp;
		sr.nh = normal;
		sr.ph = ray.o + t * ray.d;
		return true;
	}
	else {
		return false;
	}
}

bool Plane::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	float t = (normal * (point - ray.o)) / (ray.d * normal);

	if (t > 0) {
		tmin = t;
		return true;
	}
	return false;
}