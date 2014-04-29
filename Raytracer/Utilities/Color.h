#ifndef __COLOR__
#define __COLOR__

class Color {
	public: 
		float r, g, b;

		Color();
		Color(float c);
		Color(float r, float g, float b);
		Color(const Color& c);
		~Color();

		Color& operator= (const Color& rhs);
		Color operator+ (const Color& c) const;
		Color& operator+= (const Color& c);
		Color operator* (const float a) const;
		Color& operator*= (const float a);
		Color operator/ (const float a) const;
		Color& operator/= (const float a);
		Color operator* (const Color& c) const;
		bool operator== (const Color& c) const;
		
		Color powc(float p) const;
		float avg() const;

		Color& clamp();
};

inline Color Color::operator+ (const Color& c) const {
	return Color(r + c.r, g + c.g, b + c.b);
}

inline Color& Color::operator+= (const Color& c) {
	r += c.r; g += c.g; b += c.b;
	return *this;
}

inline Color Color::operator* (const float a) const {
	return Color(r * a, g * a, b * a);
}

inline Color& Color::operator*= (const float a) {
	r *= a; g *= a; b *= a;
	return *this;
}

inline Color Color::operator/ (const float a) const {
	return Color(r / a, g / a, b / a);
}

inline Color& Color::operator/= (const float a) {
	r /= a; g /= a; b /= a;
	return *this;
}

inline Color Color::operator* (const Color& c) const {
	return Color(r * c.r, g * c.g, b * c.b);
}

inline bool Color::operator== (const Color& c) const {
	return r == c.r && g == c.g && b == c.b;
}

inline float Color::avg() const {
	return 0.3333333333f * (r + g + b);
}

Color operator* (const float a, const Color& c);
inline Color operator* (const float a, const Color& c) {
	return Color(a * c.r, a * c.g, a * c.b);
}

#endif