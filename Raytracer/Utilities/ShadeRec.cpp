#include "Constants.h"
#include "ShadeRec.h"

ShadeRec::ShadeRec(const World& w) : hit_obj(false), material_ptr(NULL), ph(Point3D(0,0,0)), nh(Normal(0,1,0)), ray(Ray()), color(black), world(w), depth(0), t(0.0), u(0.0), v(0.0), accumulate(false)
{}

ShadeRec::ShadeRec(const ShadeRec& sr) : hit_obj(sr.hit_obj), material_ptr(sr.material_ptr), ph(sr.ph), local_ph(sr.local_ph), nh(sr.nh), ray(sr.ray), color(sr.color), world(sr.world), depth(0), t(sr.t), u(sr.u), v(sr.v), accumulate(sr.accumulate)
{}

ShadeRec::~ShadeRec()
{
	// TODO: this breaks things...
	/*
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}
	*/
}