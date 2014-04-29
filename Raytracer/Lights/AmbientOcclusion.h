#ifndef __AMBIENTOCCLUSION__
#define __AMBIENTOCCLUSION__

#include "Light.h"
#include "../Geometry/Geodesic.h"

class AmbientOcclusion : public Light {
	public:
		AmbientOcclusion();

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);

	private:
		Geodesic* geodesic_ptr;
		Point3D sample_point;
		Normal normal;
		Vector3D u, v, w;
		float ls;
		Color color;
};

#endif