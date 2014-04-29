#ifndef __DENSITY__
#define __DENSITY__

#include "Volumetric.h"
#include <vector>

class Density : public Volumetric {
	public:
		Density();
		Density(const char* filename);

		virtual float evaluate(const Point3D& point) const;
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

	private:
		float h;
		float scale;
		int dim_x;
		int dim_y;
		int dim_z;
		std::vector<float> data;
};

#endif