#include "RayCast.h"
#include "../World/World.h"

RayCast::RayCast() : Tracer()
{}

RayCast::RayCast(World* wp) : Tracer(wp)
{}

Color RayCast::trace_ray(const Ray& ray) const {
	ShadeRec sr = world_ptr->hit_objects(ray);
	
	if (sr.hit_obj) {
		sr.ray = ray;
		return sr.material_ptr->shade(sr);
	}
	else {
		return world_ptr->get_bgcolor();
	}
}