#include <math.h>
#include "Color.h"

Color::Color() : r(0.0), g(0.0), b(0.0)
{}

Color::Color(float c) : r(c), g(c), b(c)
{}

Color::Color(float r, float g, float b) : r(r), g(g), b(b)
{}

Color::Color(const Color& c) : r(c.r), g(c.g), b(c.b)
{}

Color::~Color()
{}

Color& Color::operator= (const Color& rhs) {
	if (this == &rhs)
		return *this;
	r = rhs.r; g = rhs.g; b = rhs.b;
	return *this;
}

Color Color::powc(float p) const {
	return Color(pow(r, p), pow(g, p), pow(b, p));
}

Color& Color::clamp() {
	if (r > 1.0) r = 1.0;
	else if (r < 0.0) r = 0.0;
	if (g > 1.0) g = 1.0;
	else if (g < 0.0) g = 0.0;
	if (b > 1.0) b = 1.0;
	else if (b < 0.0) b = 0.0;

	return *this;
}