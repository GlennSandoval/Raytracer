#ifndef __TEXTURE__
#define __TEXTURE__

#include "../Utilities/ShadeRec.h"

class Texture {
	public:
		Texture();
		virtual Color get_color(const ShadeRec& sr) const = 0;
};

#endif