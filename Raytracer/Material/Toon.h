#ifndef __TOON__
#define __TOON__

#include "Material.h"

class Toon : public Material {
	public:
		Toon();
		virtual Color shade(ShadeRec& sr);
		void set_warm_color(const Color& c);
		void set_cool_color(const Color& c);

	private:
		Color warm_color;
		Color cool_color;
};

inline void Toon::set_warm_color(const Color& c) {
	warm_color = c;
}

inline void Toon::set_cool_color(const Color& c) {
	cool_color = c;
}

#endif