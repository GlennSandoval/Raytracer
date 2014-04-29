#include <math.h>
#include "Sphere.h"
#include "../Utilities/Constants.h"

Sphere::Sphere() : center(Point3D(0,0,0)), radius(1), blur(false)
{}

Sphere::Sphere(const Point3D c, const float r) : center(c), radius(r), blur(false)
{}

void Sphere::set_center(const Point3D p) {
	center = p;
}

void Sphere::set_radius(const float r) {
	radius = r;
}

bool Sphere::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	Point3D newCenter = center;
	if (blur) {
		float time = ((float)rand()) / (RAND_MAX + 1.0);
		Vector3D vel = (time * Vector3D(0, 1, 0));
		newCenter = center + vel;
	}

	Vector3D d = newCenter - ray.o;
	Vector3D npe = ray.d.normalized();
	float b = d * npe;
	float c = d * d - (radius * radius);

	float temp = b - sqrt( (b * b) - c );
	if (temp > 0.0 && temp < tmin) {
		t = temp;
		tmin = t;
		sr.ph = ray.o + t * npe;
		sr.nh = (sr.ph - newCenter) / radius;
		return true;
	}

	temp = b + sqrt( (b * b) - c );
	if (temp > 0.0 && temp < tmin) {
		t = temp;
		tmin = t;
		sr.ph = ray.o + t * npe;
		sr.nh = (sr.ph - newCenter) / radius;
		return true;
	}
	
	return false;
}

bool Sphere::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	Vector3D d = center - ray.o;
	Vector3D nlh = ray.d.normalized();
	float b = d * nlh;
	float c = d * d - (radius * radius);

	float t = b - sqrt( (b * b) - c );
	if (t > 0.1) {
		Li += 2 * sqrt( (b * b) - c );
		tmin = t;
		return true;
	}

	t = b + sqrt( (b * b) - c );
	if (t > 0.1) {
		Li += 2 * sqrt( (b * b) - c );
		tmin = t;
		return true;
	}

	return false;
}

Point3D Sphere::translate_to_local_coords(const Point3D& p) const {
	return (p - center) / radius;
}
