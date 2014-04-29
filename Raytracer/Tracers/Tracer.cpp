#include "Tracer.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Color.h"

Tracer::Tracer()
{}

Tracer::Tracer(World* wp) : world_ptr(wp)
{}

Color Tracer::trace_ray(const Ray& ray) const {
	return black;
}