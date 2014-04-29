#ifndef __PLANARMAPPING__
#define __PLANARMAPPING__

#include "Mapping.h"
#include "../Utilities/Point3D.h"

class PlanarMapping : public Mapping{
	public:
		virtual void get_pixel_coordinates(const ShadeRec& sr, const int hres, const int vres, int& row, int& col);
};

#endif