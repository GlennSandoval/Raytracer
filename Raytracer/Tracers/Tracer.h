#ifndef __TRACER__
#define __TRACER__

#include "../Utilities/Ray.h"
#include "../World/World.h"

class Tracer {
	public:
		Tracer();
		Tracer(World* wp);

		virtual Color trace_ray(const Ray& ray) const;

	protected:
		World* world_ptr;
};

#endif