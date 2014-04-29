#ifndef __RAYCAST__
#define __RAYCAST__

#include "Tracer.h"

class RayCast : public Tracer {
	public:
		RayCast();
		RayCast(World* wp);

		virtual Color trace_ray(const Ray& ray) const;
};

#endif