#ifndef __IRIDESCENT__
#define __IRIDESCENT__

#include "Phong.h"
#include <vector>

class Iridescent : public Phong {
	public:
		Iridescent();
		virtual Color shade(ShadeRec& sr);
		Color WLToColor(float wl);

		float eta;

		float WL_HIGH;
		float WL_LOW;
		float WL_SKIP;
		float NM;
		float n;
		float d;
		std::vector<Color> colors;
		std::vector<float> wls;
};

#endif