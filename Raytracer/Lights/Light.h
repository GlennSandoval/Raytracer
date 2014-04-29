#ifndef __LIGHT__
#define __LIGHT__

#include "../Utilities/Vector3D.h"
#include "../Utilities/ShadeRec.h"

class Light {
	public:
		Light();

		void set_shadows(const bool sh);
		bool casts_shadows() const;
		virtual Vector3D get_direction(const ShadeRec& sr) = 0;
		virtual Color L(const ShadeRec& sr) = 0;
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) = 0;
		virtual Point3D get_position() const;
	private:
		bool shadows;
};

inline Point3D Light::get_position() const {
	return Point3D(0,0,0);
}

inline void Light::set_shadows(const bool sh) {
	shadows = sh;
}

inline bool Light::casts_shadows() const {
	return shadows;
}

#endif