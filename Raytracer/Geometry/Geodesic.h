#ifndef __GEODESIC__
#define __GEODESIC__

#include "GeometricObject.h"
#include "../Utilities/Ray.h"
#include "../Utilities/Point3D.h"
#include <vector>

class Geodesic : public GeometricObject{
	public:
		Geodesic();
		Geodesic(const char* filename);

		std::vector<std::string> split(const std::string &s, char delim);
		Point3D get_sample();

		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

	private:
		std::vector<Point3D*> points;
		int count;
};

inline Point3D Geodesic::get_sample() {
	return *points[count++ % points.size()];
}

#endif