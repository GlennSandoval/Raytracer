#include "Rect.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Point2D.h"

Rect::Rect() : p0(-4,-1,-1), a(0,0,2), b(0,2,0), normal(1,0,0), sampler_ptr(NULL), area(4.0), inv_area(0.25)
{}

Rect::Rect(const Point3D& p, const Vector3D& v1, const Vector3D& v2) : GeometricObject(), p0(p), a(v1), b(v2), area(a.mag() * b.mag()), inv_area(1.0 / area), sampler_ptr(NULL)
{
	normal = a ^ b;
	normal.normalize();
}

Rect::Rect(const Point3D& p, const Vector3D& v1, const Vector3D& v2, const Normal& n) : GeometricObject(), p0(p), a(v1), b(v2), normal(n), area(a.mag() * b.mag()), inv_area(1.0 / area), sampler_ptr(NULL)
{
	normal.normalize();
}

bool Rect::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {	
	float temp = (p0 - ray.o) * normal / (ray.d * normal);

	if (temp <= 0.001) return false;

	Point3D p = ray.o + temp * ray.d;
	Vector3D d = p - p0;

	float ddota = d * a;
	if (ddota < 0.0 || ddota > a.mag_sqr()) return false;

	float ddotb = d * b;
	if (ddotb < 0.0 || ddotb > b.mag_sqr()) return false;

	if (temp > tmin) return false;
	
	t = temp;
	tmin = t;
	sr.nh = normal;
	sr.ph = p;

	return true;
}

bool Rect::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	float t = (p0 - ray.o) * normal / (ray.d * normal);

	if (t <= 0.001) return false;

	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - p0;

	float ddota = d * a;
	if (ddota < 0.0 || ddota > a.mag_sqr()) return false;

	float ddotb = d * b;
	if (ddotb < 0.0 || ddotb > b.mag_sqr()) return false;

	tmin = t;

	return true;
}

Point3D Rect::sample() {
	Point2D sample_point = sampler_ptr->sample_unit_square();
	int n = sampler_ptr->get_num_samples();
	int r1 = (rand() % (n + 1)) / n;
	int r2 = (rand() % (n + 1)) / n;

	return (p0 + (sample_point.x + r1) * a + (sample_point.y + r2) * b);
}
