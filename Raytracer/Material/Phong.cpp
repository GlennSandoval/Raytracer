#include "Phong.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Phong::Phong() : color(black), specCoeff(10)
{
	texture_ptr = NULL;
	normalmap_ptr = NULL;
}

Color Phong::shade(ShadeRec& sr) {
	Color col = black;
	float s = 0.0;
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
		Vector3D npe = sr.ray.d.normalized();
		Vector3D v = -sr.ray.d;
		Vector3D r = sr.ray.d + 2.0f * (sr.nh * v) * sr.nh;

		float cosTheta = sr.nh * nlh;
		float cosPsi = nlh * r;

		s = (cosPsi + 1) / 2;
		c = (cosTheta + 1) / 2;
		//c = cosTheta;
		if (c < 0) c = 0;
		if (c > 1) c = 1;

		s = pow(s, specCoeff);

		if (s < 0) s = 0;
		if (s > 1) s = 1;

		Color csh = sr.world.get_light(i)->L(sr);

		col += ((1 - s)*(color * c) + s * white) * csh;
	}
	return col;
}
