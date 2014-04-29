#include "Lambert.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Lambert::Lambert()
{
	texture_ptr = NULL;
	normalmap_ptr = NULL;
}

Color Lambert::shade(ShadeRec& sr) {
	Color col = black;
	float c = 0.0;

	// get the texture
	if (texture_ptr) {
		color = texture_ptr->get_color(sr);
	}
	// get the normal map
	if (normalmap_ptr) {
		Color norm = normalmap_ptr->get_color(sr);
		float d0 = 2 * norm.r - 1;
		float d1 = 2 * norm.g - 1;

		Point3D p10 = Point3D( sin( TWO_PI * (sr.u + 1)) * sin( sr.v ), 
														cos( TWO_PI * (sr.u + 1)) * sin( sr.v ),
														cos( sr.v ) );

		Point3D p01 = Point3D( sin( TWO_PI * sr.u) * sin( sr.v + 1 ),
														cos( TWO_PI * sr.u) * sin( sr.v + 1 ),
														cos( sr.v + 1) );

		Vector3D v0 = p10 - sr.ph;
		v0.normalize();
		Vector3D v1 = p01 - sr.ph;
		v1.normalize();

		sr.nh = sr.nh + d0 * v0 + d1 * v1;
		sr.nh.normalize();
	}

	for (int i = 0; i < sr.world.get_num_lights(); i++) {		
		Vector3D nlh = sr.world.get_light(i)->get_direction(sr);
		Ray shRay = Ray(sr.ph, nlh);
        
		float cosTheta = sr.nh * nlh;
		c = cosTheta;
		if (c < 0) c = 0;
		if (c > 1) c = 1;

		Color csh = sr.world.get_light(i)->L(sr);
		col += color * c * csh;
	}
	return col;
}