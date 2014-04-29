#ifndef __JULIASET__
#define __JULIASET__

#include "Volumetric.h"

class JuliaSet : public Volumetric {
	public:
		JuliaSet();

		virtual float evaluate(const Point3D& point) const;
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

	private:
		int max_iter;
};

#endif