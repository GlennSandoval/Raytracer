#include "CheckerTexture.h"
#include "../Utilities/Constants.h"
#include <math.h>

CheckerTexture::CheckerTexture() : color1(black), color2(white), size(10.0)
{}

CheckerTexture::CheckerTexture(Color c1, Color c2) : color1(c1), color2(c2), size(10.0)
{}

Color CheckerTexture::get_color(const ShadeRec& sr) const {
	float x = sr.local_ph.x;
	float y = sr.local_ph.y;
	float z = sr.local_ph.z;

	int r = (int)floor(x / size) + (int)floor(y / size) + (int)floor(z / size);

	if (r % 2 == 0) 
		return color1;
	else 
		return color2;
}