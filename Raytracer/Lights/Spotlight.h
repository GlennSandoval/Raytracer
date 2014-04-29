#ifndef __SPOTLIGHT__
#define __SPOTLIGHT__

#include "Light.h"
#include "../Texture/ImageTexture.h"

class Spotlight : public Light {
	public:
		Spotlight();

		void set_color(const Color& c);
		void set_direction(const Vector3D& v);
		void set_radiance(const float rad);
		void set_location(const Point3D& p);

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
		virtual Point3D get_position() const;

		void set_texture(ImageTexture* tp);
	private:
		float ls;
		Color color;
		Point3D location;
		Vector3D direction;
		float alpha;
		float alpha2;
		ImageTexture* tex_ptr;

};

inline void Spotlight::set_texture(ImageTexture* tp) {
	tex_ptr = tp;
}

inline void Spotlight::set_color(const Color& c) {
	color = c;
}

inline void Spotlight::set_direction(const Vector3D& v) {
	direction = v;
}

inline void Spotlight::set_radiance(const float rad) {
	ls = rad;
}

inline void Spotlight::set_location(const Point3D& p) {
	location = p;
}

#endif