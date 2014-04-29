#ifndef __EMISSIVE__
#define __EMISSIVE__

#include "Material.h"

class Emissive : public Material {
	public:
		Emissive();
		void set_color(const Color& c);
		void scale_radiance(const float rad);

		virtual Color L(ShadeRec& sr) const;
		virtual Color get_Le(const ShadeRec& sr) const;
		virtual Color shade(ShadeRec& sr);

	private:
		float radiance;
		Color color;
};

inline void Emissive::set_color(const Color& c) {
	color = c;
}

inline void Emissive::scale_radiance(const float rad) {
	radiance *= rad;
}

inline Color Emissive::L(ShadeRec& sr) const {
	return radiance * color;
}

inline Color Emissive::get_Le(const ShadeRec& Sr) const {
	return radiance * color;
}

#endif