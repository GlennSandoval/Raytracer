#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

class Plane : public GeometricObject {
	public:
		Plane();
		Plane(const Point3D p, const Normal& n);
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

	private:
		Point3D point;
		Normal normal;
};

#endif