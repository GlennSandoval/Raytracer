#ifndef __GOOCH__
#define __GOOCH__

#include "Material.h"

class Gooch : public Material {
	public:
		Gooch();
		virtual Color shade(ShadeRec& sr);
		void set_warm_color(const Color& c);
		void set_cool_color(const Color& c);

	private:
		Color warm_color;
		Color cool_color;
};

inline void Gooch::set_warm_color(const Color& c) {
	warm_color = c;
}

inline void Gooch::set_cool_color(const Color& c) {
	cool_color = c;
}

#endif