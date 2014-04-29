#ifndef __TRANSLUCENT__
#define __TRANSLUCENT__

#include "Phong.h"

class Translucent : public Phong {
	public:
		Translucent();
		virtual Color shade(ShadeRec& sr);
		float eta;
};

#endif