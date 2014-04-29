#ifndef __VolumetricFire__
#define __VolumetricFire__

#include "Volumetric.h"
#include "../Simulations/fire.h"
#include <vector>

class VolumetricFire : public Volumetric {
	public:
		VolumetricFire();
		VolumetricFire(const char* filename);
		VolumetricFire(Fire& f);

		virtual float evaluate(const Point3D& point) const;
		float blackbody_spectrum(float wavelength, float temp) const;
		Color XYZtoRGB(const float &X, const float &Y, const float &Z) const;
		Color calculate_blackbody(const float &temp) const;
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;
		int I(int x, int y, int z) const;
		float getSmoke(const Point3D& point) const;
		float getFuel(const Point3D& point) const;

	private:
		float h;
		float scale;
		int dim_x;
		int dim_y;
		int dim_z;
		std::vector<float> data;
		std::vector<float> smoke;
		std::vector<float> fuel;
		Color smokeColor;
		Fire& fire;
};

#endif