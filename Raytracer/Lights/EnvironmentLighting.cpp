#include "EnvironmentLighting.h"
#include "../Utilities/Constants.h"
#include "../World/World.h"

EnvironmentLighting::EnvironmentLighting() : Light(), ls(6.0), tex_ptr(NULL), map_ptr(NULL)
{
	geodesic_ptr = new Geodesic("SampleAssets/objs/642.obj");
}

EnvironmentLighting::EnvironmentLighting(const char* filename) : Light(), ls(6.0)
{
	geodesic_ptr = new Geodesic("SampleAssets/objs/642.obj");
	tex_ptr = new ImageTexture(filename);
	map_ptr = new SphericalMapping;
	tex_ptr->set_mapping(map_ptr);
}

Vector3D EnvironmentLighting::get_direction(const ShadeRec& sr) {
	u = sr.world.get_camera().u;
	v = sr.world.get_camera().v;
	w = sr.world.get_camera().w;

	sample_point = geodesic_ptr->get_sample();
	return (sample_point.x * u + sample_point.y * v + sample_point.z * w).normalized();
}

Color EnvironmentLighting::L(const ShadeRec& sr) {
	float Li = 0.0;

	Ray ray = Ray(sr.ph, get_direction(sr)); // construct a shadow ray from hit point toward light

	if (in_shadow(ray, sr))
		return black;
	
	ShadeRec sr2 = ShadeRec(sr);
	sr2.local_ph.x = u * ray.d.normalized();
	sr2.local_ph.y = v * ray.d.normalized();
	sr2.local_ph.z = w * ray.d.normalized();
	Color color = tex_ptr->get_color(sr2);
	return ls * color;
}

bool EnvironmentLighting::in_shadow(const Ray& ray, const ShadeRec& sr) {
	float t;
	float d = (sample_point - ray.o) * ray.d;
	float Li = 0.0;

	for (unsigned int i = 0; i < sr.world.get_num_objects(); i++) {
		if (sr.world.get_object(i)->shadow_hit(ray, t, Li)) {
			return true;
		}
	}
	return false;
}