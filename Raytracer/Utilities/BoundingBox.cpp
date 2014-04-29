#include "BoundingBox.h"

BoundingBox::BoundingBox() : x_min(-1), x_max(1), y_min(-1), y_max(1), z_min(-1), z_max(1)
{}

BoundingBox::BoundingBox( const float x0, const float x1, const float y0, const float y1, const float z0, const float z1)
	: x_min(x0), x_max(x1), y_min(y0), y_max(y1), z_min(z0), z_max(z1)
{}

BoundingBox::BoundingBox( const Point3D p0, const Point3D p1 )
	: x_min(p0.x), x_max(p1.x), y_min(p0.y), y_max(p1.y), z_min(p0.z), z_max(p1.z)
{
	z_min = p0.z;
	z_max = p1.z;
}

BoundingBox::BoundingBox( const BoundingBox& bbox )
	: x_min(bbox.x_min), x_max(bbox.x_max), y_min(bbox.y_min), y_max(bbox.y_max), z_min(bbox.z_min), z_max(bbox.z_max)
{}

BoundingBox& BoundingBox::operator=(const BoundingBox& rhs) {
	if (this == &rhs)
		return *this;

	x_min = rhs.x_min;
	x_max = rhs.x_max;
	y_min = rhs.y_min;
	y_max = rhs.y_max;
	z_min = rhs.z_min;
	z_max = rhs.z_max;

	return *this;
}

void BoundingBox::expand( const BoundingBox& bbox ) {
	x_min = std::min(x_min, bbox.x_min);
	x_max = std::max(x_max, bbox.x_max);
	y_min = std::min(y_min, bbox.y_min);
	y_max = std::max(y_max, bbox.y_max);
	z_min = std::min(z_min, bbox.z_min);
	z_max = std::max(z_max, bbox.z_max);
}

int BoundingBox::longest_axis() const {
	float x_length = x_max - x_min;
	float y_length = y_max - y_min;
	float z_length = z_max - z_min;

	if (x_length >= y_length && x_length >= z_length)
		return 0;
	if (y_length >= x_length && y_length >= z_length)
		return 1;
	if (z_length >= x_length && z_length >= y_length)
		return 2;
	return -1;
}

bool BoundingBox::hit(const Ray& ray) const {
	float tx_min, ty_min, tz_min, tx_max, ty_max, tz_max;
	
	float a = 1.0 / ray.d.x;
	if (a >= 0) {
		tx_min = (x_min - ray.o.x) * a;
		tx_max = (x_max - ray.o.x) * a;
	}
	else {
		tx_min = (x_max - ray.o.x) * a;
		tx_max = (x_min - ray.o.x) * a;
	}

	float b = 1.0 / ray.d.y;
	if (b >= 0) {
		ty_min = (y_min - ray.o.y) * b;
		ty_max = (y_max - ray.o.y) * b;
	}
	else {
		ty_min = (y_max - ray.o.y) * b;
		ty_max = (y_min - ray.o.y) * b;
	}

	float c = 1.0 / ray.d.z;
	if (c >= 0) {
		tz_min = (z_min - ray.o.z) * c;
		tz_max = (z_max - ray.o.z) * c;
	}
	else {
		tz_min = (z_max - ray.o.z) * c;
		tz_max = (z_min - ray.o.z) * c;
	}

	float t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	return (t0 < t1 && t1 > 0.001);
}

bool BoundingBox::inside(const Point3D& p) const {
	return ((p.x > x_min && p.x < x_max) &&
			(p.y > y_min && p.y < y_max) &&
			(p.z > z_min && p.z < z_max));
}