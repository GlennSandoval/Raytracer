#include "PlanarMapping.h"
#include "../Utilities/Constants.h"
#include <math.h>
#include "../World/World.h"

void PlanarMapping::get_pixel_coordinates(const ShadeRec& sr, const int hres, const int vres, int& row, int& col) {
	float x = (sr.local_ph * sr.world.get_camera().u) / 6.0f; //(sr.local_ph.z+1) / 2;//((sr.local_ph - 2*sr.world.camera.p0) * sr.world.camera.u) / 6.0;
	float y = (sr.local_ph * sr.world.get_camera().v) / 6.0f; //(sr.local_ph.x+1) / 2;//((sr.local_ph - 2*sr.world.camera.p0) * sr.world.camera.v) / 6.0;

	float u, v;

	// single
	/*if (0 < x && x < 1) { 
		u = x;
	}
	if (0 < y && y < 1) { 
		v = y;
	}*/

	// wallpaper
	u = x - (int)x;
	u = (u < 0) ? u + 1 : u;
	
	v = y - (int)y;
	v = (v < 0) ? v + 1 : v;

	col = (int) (hres - 1) * u;
	row = (int) (vres - 1) * v;
}