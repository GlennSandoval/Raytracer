#ifndef __MAPPING__
#define __MAPPING__

#include "../Utilities/ShadeRec.h"

class Mapping {
	public:
		virtual void get_pixel_coordinates(const ShadeRec& sr, const int hres, const int vres, int& row, int& col) = 0;
};

#endif