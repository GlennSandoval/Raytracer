#include "Point2D.h"

Point2D::Point2D() : x(0.0), y(0.0)
{}

Point2D::Point2D(const float a) : x(a), y(a)
{}

Point2D::Point2D(const float x, const float y) : x(x), y(y)
{}

Point2D::Point2D(const Point2D& p) : x(p.x), y(p.y)
{}

Point2D::~Point2D()
{}

Point2D& Point2D::operator= (const Point2D& rhs) {
	if (this == &rhs)
		return *this;
	x = rhs.x; y = rhs.y;
	return *this;
}