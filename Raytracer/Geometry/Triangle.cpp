#include "Triangle.h"

Triangle::Triangle() : p0(new Point3D(0,0,0)), p1(new Point3D(1,0,0)), p2(new Point3D(0,1,0))
{
	float min_x = std::min(std::min(p0->x, p1->x), p2->x) - 0.1f;
	float max_x = std::max(std::max(p0->x, p1->x), p2->x) + 0.1f;
	float min_y = std::min(std::min(p0->y, p1->y), p2->y) - 0.1f;
	float max_y = std::max(std::max(p0->y, p1->y), p2->y) + 0.1f;
	float min_z = std::min(std::min(p0->z, p1->z), p2->z) - 0.1f;
	float max_z = std::max(std::max(p0->z, p1->z), p2->z) + 0.1f;

	bbox = BoundingBox(min_x, max_x, min_y, max_y, min_z, max_z);
	
	normal = Vector3D(0,0,1);
}

Triangle::Triangle(Point3D* pa, Point3D* pb, Point3D* pc) : p0(pa), p1(pb), p2(pc)
{
	Vector3D v1 = *p1 - *p0;
	Vector3D v2 = *p2 - *p0;
	Vector3D v0 = v1 ^ v2;

	float min_x = std::min(std::min(p0->x, p1->x), p2->x) - 0.1f;
	float max_x = std::max(std::max(p0->x, p1->x), p2->x) + 0.1f;
	float min_y = std::min(std::min(p0->y, p1->y), p2->y) - 0.1f;
	float max_y = std::max(std::max(p0->y, p1->y), p2->y) + 0.1f;
	float min_z = std::min(std::min(p0->z, p1->z), p2->z) - 0.1f;
	float max_z = std::max(std::max(p0->z, p1->z), p2->z) + 0.1f;

	bbox = BoundingBox(min_x, max_x, min_y, max_y, min_z, max_z);

	normal = v0.normalized();
}

bool Triangle::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	float temp = ((*p0 - ray.o) * normal) / (ray.d * normal);

	if (temp > 0.1) {
		Point3D ph = ray.o + temp * ray.d;

		Vector3D A = (*p1 - *p0) ^ (*p2 - *p0);
		if (A.mag() == 0) return false;

		Vector3D A0 = (ph - *p1) ^ (ph - *p2);
		Vector3D A1 = (ph - *p2) ^ (ph - *p0);
		Vector3D A2 = (ph - *p0) ^ (ph - *p1);

		if(A0*normal > 0 && A1*normal >0 && A2*normal > 0)
		{
			float w0 = A0.mag() / A.mag();
			float w1 = A1.mag() / A.mag();
			float w2 = A2.mag() / A.mag();

			if (0 < w1 && 0 < w2 && w1 + w2 < 1 && temp < tmin) {
				t = temp;
				tmin = t;
				sr.ph = ph;
				sr.nh = w0 * p0->normal + w1 * p1->normal + w2 * p2->normal;
				sr.nh.normalize();
				sr.u = w0 * uv0.x + w1 * uv1.x + w2 * uv2.x;
				sr.v = w0 * uv0.y + w1 * uv1.y + w2 * uv2.y;
				return true;
			}
		}
	}
	return false;
}

bool Triangle::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	float t = ((*p0 - ray.o) * normal) / (ray.d * normal);

	if (t > 0.1) {
		Point3D ph = ray.o + t * ray.d;

		Vector3D A = (*p1 - *p0) ^ (*p2 - *p0);
		if (A.mag() == 0) return false;

		Vector3D A0 = (ph - *p1) ^ (ph - *p2);
		Vector3D A1 = (ph - *p2) ^ (ph - *p0);
		Vector3D A2 = (ph - *p0) ^ (ph - *p1);

		if(A0*normal > 0 && A1*normal >0 && A2*normal > 0)
		{
			float w0 = A0.mag() / A.mag();
			float w1 = A1.mag() / A.mag();
			float w2 = A2.mag() / A.mag();

			if (0 < w1 && 0 < w2 && w1 + w2 < 1) {
				tmin = t;
				return true;
			}
		}
	}
	return false;
}
