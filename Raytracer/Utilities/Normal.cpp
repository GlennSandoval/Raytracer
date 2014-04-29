#include <math.h>
#include "Normal.h"

Normal::Normal(void) : x(0.0), y(0.0), z(0.0)							
{}

Normal::Normal(float a) : x(a), y(a), z(a)							
{}

Normal::Normal(float x, float y, float z)	: x(x), y(y), z(z)
{}

Normal::Normal(const Normal& n) : x(n.x), y(n.y), z(n.z)
{}

Normal::Normal(const Vector3D& v) : x(v.x), y(v.y), z(v.z)  
{}

Normal::~Normal () 							
{}

Normal& Normal::operator= (const Normal& rhs) {
	if (this == &rhs)
		return *this;

	x = rhs.x; y = rhs.y; z = rhs.z;

	return *this;
}

Normal& Normal::operator= (const Vector3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return *this;
}

Normal& Normal::operator= (const Point3D& rhs) {		
	x = rhs.x; y = rhs.y; z = rhs.z;
	return *this;
}

void Normal::normalize() {	
	float length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}

Normal operator* (const Matrix& matrix, const Normal& n) {
	return Normal(	matrix.m[0][0] * n.x + matrix.m[1][0] * n.y + matrix.m[2][0] * n.z,
								matrix.m[0][1] * n.x + matrix.m[1][1] * n.y + matrix.m[2][1] * n.z,
								matrix.m[0][2] * n.x + matrix.m[1][2] * n.y + matrix.m[2][2] * n.z);
}
