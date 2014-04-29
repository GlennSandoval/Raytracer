#ifndef __LAMBERT__
#define __LAMBERT__

#include "Material.h"

class Lambert : public Material {
	public:
		Lambert();
		virtual Color shade(ShadeRec& sr);
		void set_color(const Color& c);

	private:
		Color color;
};


inline void Lambert::set_color(const Color& c) {
	color = c;
}

#endif