#ifndef __RECT__
#define __RECT__

#include "GeometricObject.h"

class Rect : public GeometricObject {
	public:
		Rect();
		Rect(const Point3D& p, const Vector3D& v1, const Vector3D& v2);
		Rect(const Point3D& p, const Vector3D& v1, const Vector3D& v2, const Normal& n);

		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;

		void set_sampler(Sampler* sampler);
		virtual Point3D sample();
		virtual float pdf(const ShadeRec& sr) const;
		virtual Normal get_normal(const Point3D& p) const;
		Sampler* get_sampler() const;

	private:
		Point3D p0;
		Vector3D a;
		Vector3D b;
		Normal normal;
		Sampler* sampler_ptr;

		float area;
		float inv_area;
};

inline Sampler* Rect::get_sampler() const {
	return sampler_ptr;
}

inline void Rect::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
}

inline float Rect::pdf(const ShadeRec& sr) const {
	return inv_area;
}

inline Normal Rect::get_normal(const Point3D& p) const {
	return normal;
}

#endif