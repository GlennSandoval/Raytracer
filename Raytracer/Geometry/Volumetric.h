#ifndef __VOLUMETRIC__
#define __VOLUMETRIC__

#include "GeometricObject.h"
#include "../Utilities/BoundingBox.h"

class Volumetric : public GeometricObject {
	public:
		Volumetric();
		Volumetric(const Point3D& point1, const Point3D& point2);

		void set_stepsize(float step);

		virtual float evaluate(const Point3D& point) const = 0;
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const = 0;

	protected:
		BoundingBox bbox;
		float stepsize;
};

inline void Volumetric::set_stepsize(float step) {
	stepsize = step;
}

#endif