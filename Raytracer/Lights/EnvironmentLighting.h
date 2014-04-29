#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include "Light.h"
#include "../Geometry/Geodesic.h"
#include "../Texture/ImageTexture.h"
#include "../Mapping/SphericalMapping.h"

class EnvironmentLighting : public Light {
	public:
		EnvironmentLighting();
		EnvironmentLighting(const char* filename);

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);

	private:
		Geodesic* geodesic_ptr;
		Point3D sample_point;
		Normal normal;
		Vector3D u, v, w;
		float ls;
		ImageTexture* tex_ptr;
		SphericalMapping* map_ptr;
};

#endif