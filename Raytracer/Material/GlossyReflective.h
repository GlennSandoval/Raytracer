#ifndef __GLOSSYREFLECTIVE__
#define __GLOSSYREFLECTIVE__

#include "Phong.h"

class GlossyReflective : public Phong {
	public:
		GlossyReflective();
		virtual Color shade(ShadeRec& sr);
		float reflectivity;
};

#endif