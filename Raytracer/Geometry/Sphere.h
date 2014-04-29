#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"

class Sphere : public GeometricObject {
	public:
		bool blur;
		Sphere();
		Sphere(const Point3D c, const float r);
		void set_center(const Point3D p);
		void set_radius(const float r);
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;
		virtual Point3D translate_to_local_coords(const Point3D& p) const;

	private:
		Point3D center;
		float radius;
};

#endif