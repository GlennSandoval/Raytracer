#ifndef __POINTLIGHT__
#define __POINTLIGHT__

#include "Light.h"

class PointLight : public Light {
	public:
		PointLight();

		void set_color(const Color& c);
		void set_location(const Point3D& p);
		void set_radiance(const float rad);

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
		virtual Point3D get_position() const;
	private:
		float ls;
		Color color;
		Point3D location;
};

inline void PointLight::set_color(const Color& c) {
	color = c;
}

inline void PointLight::set_location(const Point3D& p) {
	location = p;
}

inline void PointLight::set_radiance(const float rad) {
	ls = rad;
}

#endif