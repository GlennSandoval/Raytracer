#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "GeometricObject.h"
#include "../Utilities/BoundingBox.h"

class Triangle : public GeometricObject {
	public:
		Triangle();
		Triangle(Point3D* pa, Point3D* pb, Point3D* pc);

		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

		BoundingBox get_bounding_box() const;
		Point3D get_midpoint() const;
		virtual Normal get_normal() const;
		void set_normal(const Vector3D& n);
		void set_vertex_normals(const Normal& norm0, const Normal& norm1, const Normal& norm2);
		void set_tex_coords(const Point2D& p20, const Point2D& p21, const Point2D& p22);
		bool has_point(const Point3D& p) const;

	private:
		BoundingBox bbox;
		Point3D* p0;
		Point3D* p1;
		Point3D* p2;
		Normal normal;
		Normal n0; // for interpolating normals stored at vertices
		Normal n1;
		Normal n2;
		Point2D uv0;
		Point2D uv1;
		Point2D uv2;
};

inline BoundingBox Triangle::get_bounding_box() const {
	return bbox;
}

inline Point3D Triangle::get_midpoint() const {
	return (1.0f / 3.0f) * Point3D(p0->x + p1->x + p2->x, p0->y + p1->y + p2->y, p0->z + p1->z + p2->z);
}

inline Normal Triangle::get_normal() const{
	return normal;
}

inline void Triangle::set_normal(const Vector3D& n) {
	normal = n;
	normal.normalize();
}

inline void Triangle::set_vertex_normals(const Normal& norm0, const Normal& norm1, const Normal& norm2) {
	n0 = norm0;
	n1 = norm1;
	n2 = norm2;
}

inline void Triangle::set_tex_coords(const Point2D& p20, const Point2D& p21, const Point2D& p22) {
	uv0 = p20;
	uv1 = p21;
	uv2 = p22;
}

inline bool Triangle::has_point(const Point3D& p) const {
	return ((p.x == p0->x && p.y == p0->y && p.z == p0->z) ||
			(p.x == p1->x && p.y == p1->y && p.z == p1->z) ||
			(p.x == p2->x && p.y == p2->y && p.z == p2->z));
}

#endif