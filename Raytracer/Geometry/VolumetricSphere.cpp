#include "VolumetricSphere.h"

VolumetricSphere::VolumetricSphere() : center(Point3D(0,0,0)), radius(1.0)
{}

VolumetricSphere::VolumetricSphere(const Point3D& c, float r) : center(c), radius(radius)
{
	bbox = BoundingBox(Point3D(c.x-r, c.y-r, c.z-r), Point3D(c.x+r, c.y+r, c.z+r));
}

float VolumetricSphere::evaluate(const Point3D& point) const {
	return (point - center).mag() - radius;
}

bool VolumetricSphere::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	// TODO: I believe this will only calculate first intersection
	// ok for sphere, not for transparent things
	if (bbox.hit(ray)) {
		Normal normal = Normal();
		Point3D hit_pt = ray.o; // + t * ray.d;

		float dist = 0.0, t=0.1;
		for (int i=0; i < 1000; i++) {
			dist = evaluate(hit_pt);
			hit_pt = ray.o + t * ray.d;
			t += stepsize;
			if (dist < 0.01) break;
		}
		if (dist < 0.01) {
			// calculate normal
			normal.x += hit_pt.x + 0.2*stepsize;
			normal.x += hit_pt.x - 0.2 * stepsize;
			normal.y += hit_pt.y + 0.2 * stepsize;
			normal.y += hit_pt.y - 0.2 * stepsize;
			normal.z += hit_pt.z + 0.2 * stepsize;
			normal.z += hit_pt.z - 0.2 * stepsize;
			normal.normalize();
			sr.ph = hit_pt;
			sr.local_ph = hit_pt;
			sr.nh = normal;
			tmin = t;
			return true;
		}
		return false;
	}
	return false;
}

bool VolumetricSphere::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return false;
}