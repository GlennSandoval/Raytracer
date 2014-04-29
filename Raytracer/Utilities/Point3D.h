#ifndef __POINT3D__
#define __POINT3D__

#include "Vector3D.h"

class Point3D {
	public:
		float x, y, z;
		Vector3D normal;
		
		Point3D();
		Point3D(const float a);
		Point3D(const float a, const float b, const float c);
		Point3D(const Point3D& p);
		Point3D(const Vector3D& v);
		~Point3D();

		Point3D& operator= (const Point3D& rhs);
		Point3D operator- () const;
		Vector3D operator- (const Point3D& p) const;
		Point3D operator+ (const Vector3D& v) const;
		Point3D operator+ (const Point3D& p) const;
		Point3D operator- (const Vector3D& v) const;
		Point3D operator* (const float a) const;

		float distance(const Point3D& p) const;
};

inline Point3D Point3D::operator- () const {
	return Point3D(-x, -y, -z);
}

inline Vector3D Point3D::operator- (const Point3D& p) const {
	return Vector3D(x - p.x, y - p.y, z - p.z);
}

inline Point3D Point3D::operator+ (const Vector3D& v) const {
	return Point3D(x + v.x, y + v.y, z + v.z);
}

inline Point3D Point3D::operator+ (const Point3D& p) const {
	return Point3D(x + p.x, y + p.y, z + p.z);
}

inline Point3D Point3D::operator- (const Vector3D& v) const {
	return Point3D(x - v.x, y - v.y, z - v.z);
}

inline Point3D Point3D::operator* (const float a) const {
	return Point3D(x * a, y * a, z * a);
}

Point3D operator* (float a, const Point3D& p);
inline Point3D operator* (float a, const Point3D& p) {
	return Point3D(a * p.x, a * p.y, a * p.z);
}

#endif