#ifndef __VOLUMETRICSPHERE__
#define __VOLUMETRICSPHERE__

#include "Volumetric.h"

class VolumetricSphere : public Volumetric {
	public:
		VolumetricSphere();
		VolumetricSphere(const Point3D& c, float r);

		virtual float evaluate(const Point3D& point) const;
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

	private:
		Point3D center;
		float radius;
};

#endif