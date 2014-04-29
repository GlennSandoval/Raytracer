#ifndef __VECTOR3D__
#define __VECTOR3D__

#include "Matrix.h"

class Normal;
class Point3D;

class Vector3D {
	public: 
		float x, y, z;

		// constructors
		Vector3D();
		Vector3D(float a);
		Vector3D(float x, float y, float z);
		Vector3D(const Vector3D& v);
		Vector3D(const Point3D& p);
		Vector3D(const Normal& n);

		~Vector3D();

		// overloaded operations
		Vector3D& operator= (const Vector3D& rhs);
		Vector3D& operator= (const Point3D& rhs);
		Vector3D& operator= (const Normal& rhs);
		Vector3D operator- (void) const;
		Vector3D operator* (const float a) const;
		Vector3D operator/ (const float a) const;
		Vector3D operator+ (const Vector3D& v) const;
		Vector3D& operator+= (const Vector3D& v);
		Vector3D operator- (const Vector3D& v) const;
		float operator* (const Vector3D& b) const;     // dot product
		Vector3D operator^ (const Vector3D& v) const;   // cross product
		
		// member functions
		float mag() const;
		float mag_sqr() const;
		void normalize();
		Vector3D normalized() const;
		Vector3D& unit();
};

inline Vector3D Vector3D::operator- (void) const {
	return Vector3D(-x, -y, -z);
}

inline Vector3D Vector3D::operator* (const float a) const {
	return Vector3D(x * a, y * a, z * a);
}

inline Vector3D Vector3D::operator/ (const float a) const {
	return Vector3D(x / a, y / a, z / a);
}

inline Vector3D Vector3D::operator+ (const Vector3D& v) const {
	return Vector3D(x + v.x , y + v.y, z + v.z);
}

inline Vector3D Vector3D::operator- (const Vector3D& v) const {
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

inline float Vector3D::operator* (const Vector3D& v) const {
	return x * v.x + y * v.y + z * v.z;
}

inline Vector3D Vector3D::operator^ (const Vector3D& v) const {
	return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

inline float Vector3D::mag_sqr() const {
	return x*x + y*y + z*z;
}

Vector3D operator* (const float a, const Vector3D& v);
inline Vector3D operator* (const float a, const Vector3D& v) {
	return Vector3D(a * v.x, a * v.y, a * v.z);
}

Vector3D operator* (const Matrix& matrix, const Vector3D& v);

#endif