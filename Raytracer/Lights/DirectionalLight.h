#ifndef __DIRECTIONALLIGHT__
#define __DIRECTIONALLIGHT__

#include "Light.h"

class DirectionalLight : public Light {
	public:
		DirectionalLight();

		void set_color(const Color& c);
		void set_direction(const Vector3D& v);
		void set_radiance(const float rad);

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
		virtual Point3D get_position() const;
	private:
		float ls;
		Color color;
		Point3D location;
		Vector3D direction;
};

inline void DirectionalLight::set_color(const Color& c) {
	color = c;
}

inline void DirectionalLight::set_direction(const Vector3D& v) {
	direction = v;
}

inline void DirectionalLight::set_radiance(const float rad) {
	ls = rad;
}

#endif