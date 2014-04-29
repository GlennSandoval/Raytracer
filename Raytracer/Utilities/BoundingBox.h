#ifndef __BBOX__
#define __BBOX__

#include <vector>
#include "Ray.h"
#include "Point3D.h"

class BoundingBox {
	public:
		float x_min, x_max, y_min, y_max, z_min, z_max;

		BoundingBox();
		BoundingBox(const float x0, const float x1,
					const float y0, const float y1,
					const float z0, const float z1);
		BoundingBox(const Point3D p0, const Point3D p1);
		BoundingBox(const BoundingBox& bbox);
		BoundingBox& operator= (const BoundingBox& rhs);
		
		void expand(const BoundingBox& bbox);
		int longest_axis() const;
		bool hit(const Ray& ray) const;
		bool inside(const Point3D& p) const;
};

#endif