#ifndef __POINT2D__
#define __POINT2D__

class Point2D {
	public: 
		float x, y;

		Point2D();
		Point2D(const float a);
		Point2D(const float x, const float y);
		Point2D(const Point2D& p);
		~Point2D();

		Point2D& operator= (const Point2D& rhs);
		Point2D operator* (const float a);
};

inline Point2D Point2D::operator* (const float a) {
	return Point2D(a * x, a * y);
}

#endif