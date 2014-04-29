#include "JuliaSet.h"

JuliaSet::JuliaSet() 
{}

float JuliaSet::evaluate(const Point3D& point) const {
	int iter=0, depth = 100;
	float n = 8;
	float x = point.x, y = point.y, z = point.z;
	
	float first_mag = x*x + y*y + z*z;
	float r=0.0, theta, phi;
	while (iter < depth && r < 8) {
		r = sqrt(x*x + y*y + z*z);
		phi = atan2(y,x);
		theta = atan2(sqrt(x*x + y*y), z);
		
		x = pow(r,n) * sin(theta*n) * cos(phi*n);
		y = pow(r,n) * sin(theta*n) * sin(phi*n);
		z = pow(r,n) * cos(theta*n);
	}
	return 0.0;
}

bool JuliaSet::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
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

bool JuliaSet::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return false;
}