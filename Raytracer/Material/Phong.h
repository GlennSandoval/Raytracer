#ifndef __PHONG__
#define __PHONG__

#include "Material.h"

class Phong : public Material {
	public:
		Phong();
		virtual Color shade(ShadeRec& sr);
		void set_color(const Color& c);
		void set_specCoeff(const float s);

	private:
		Color color;
		float specCoeff;
};


inline void Phong::set_color(const Color& c) {
	color = c;
}

inline void Phong::set_specCoeff(const float s) {
	specCoeff = s;
}

#endif