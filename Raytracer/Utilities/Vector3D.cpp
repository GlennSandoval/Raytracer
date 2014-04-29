#include <math.h>
#include "Vector3D.h"
#include "Normal.h"
#include "Point3D.h"

Vector3D::Vector3D() : x(0.0), y(0.0), z(0.0)
{}

Vector3D::Vector3D(float a) : x(a), y(a), z(a)
{}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
{}

Vector3D::Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z)
{}

Vector3D::Vector3D(const Point3D& p) : x(p.x), y(p.y), z(p.z)
{}

Vector3D::Vector3D(const Normal& n) : x(n.x), y(n.y), z(n.z)
{}

Vector3D::~Vector3D()
{}

// operators
Vector3D& Vector3D::operator= (const Vector3D& rhs) {
	if (this == &rhs) 
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return *this;
}

Vector3D& Vector3D::operator= (const Point3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;

	return *this;
}

Vector3D& Vector3D::operator= (const Normal& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;

	return *this;
}

float Vector3D::mag() const {
	return sqrt(x*x + y*y + z*z);
}

void Vector3D::normalize() {
	float mag = this->mag();
	x /= mag; y /= mag; z /= mag;
}

Vector3D Vector3D::normalized() const{
	float mag = this->mag();
	Vector3D result = Vector3D(x / mag, y / mag, z / mag);
	return  result;
}


Vector3D& Vector3D::unit() {
	float mag = this->mag();
	x /= mag; y /= mag; z /= mag;
	return *this;
}

Vector3D operator* (const Matrix& matrix, const Vector3D& v) {
return Vector3D( matrix.m[0][0] * v.x + matrix.m[0][1] * v.y + matrix.m[0][2] * v.z, 
							matrix.m[1][0] * v.x + matrix.m[1][1] * v.y + matrix.m[1][2] * v.z, 
							matrix.m[2][0] * v.x + matrix.m[2][1] * v.y + matrix.m[2][2] * v.z);
}