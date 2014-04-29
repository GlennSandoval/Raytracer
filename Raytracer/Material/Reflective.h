#ifndef __REFLECTIVE__
#define __REFLECTIVE__

#include "Phong.h"

class Reflective : public Phong {
	public:
		Reflective();
		virtual Color shade(ShadeRec& sr);
		float reflectivity;
};

#endif