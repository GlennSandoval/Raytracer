#include "Density.h"
#include <iostream>
#include <fstream>
#include <string>

Density::Density()
{}

Density::Density(const char* filename) : scale(2.0) {
	bbox = BoundingBox();
	std::ifstream file;
	file.open(filename);

	int dim;
	file >> dim;
	dim_x = dim;
	file >> dim;
	dim_y = dim;
	file >> dim;
	dim_z = dim;

	for (int i=0; i < dim_x * dim_y * dim_z; i++) {
		file >> dim;
		data.push_back(dim);
	}

	file.close();
	std::cout << "Density data read" << std::endl;

	stepsize = scale / dim_x;
}

float Density::evaluate(const Point3D& point) const {
	Point3D p = Point3D(point.x + 1.0, point.y + 1.0, point.z + 1.0);
	int index = (p.z * dim_z * dim_y) + (p.y * dim_x) + p.x;
	if (index < 0 || index > data.size()) {
		return -1;
	}	
	return data[index];
}

bool Density::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	// TODO: I believe this will only calculate first intersection
	if (bbox.hit(ray)) {
		Normal normal = Normal();
		Point3D hit_pt = ray.o; // + t * ray.d;

		float dens = 0.0, t=0.1;
		for (int i=0; i < 1000; i++) {
			if (bbox.inside(hit_pt)) {
				dens = evaluate(hit_pt);
			}
			hit_pt = ray.o + t * ray.d;
			t += stepsize;
			if (dens > 0) break;
		}
		if (dens > 0) {
			// calculate normal
			normal.x += hit_pt.x + 0.2 * stepsize;
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

bool Density::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return false;
}