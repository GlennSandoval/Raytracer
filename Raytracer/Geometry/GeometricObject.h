#ifndef __GEO__
#define __GEO__

#include "../Utilities/Point3D.h"
#include "../Utilities/Ray.h"
#include "../Utilities/Color.h"
#include "../Utilities/ShadeRec.h"
#include "../Material/Material.h"
#include "../Samplers/Sampler.h"

class GeometricObject {
	public:
		Material* get_material() const;
		void set_material(Material* m);
		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const = 0;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const = 0;

		virtual Point3D sample();
		virtual Normal get_normal() const;
		virtual Normal get_normal(const Point3D& p) const;
		virtual Point3D translate_to_local_coords(const Point3D& p) const;

	protected:
		Material* material_ptr;
		Vector3D n0;
		Vector3D n1;
		Vector3D n2;
};

inline Material* GeometricObject::get_material() const {
	return material_ptr;
}

inline void GeometricObject::set_material(Material* m) {
	material_ptr = m;
}

inline Point3D GeometricObject::sample() {
	return Point3D(0,0,0);
}

inline Normal GeometricObject::get_normal() const {
	return Normal();
}

inline Normal GeometricObject::get_normal(const Point3D& p) const {
	return Normal();
}

inline Point3D GeometricObject::translate_to_local_coords(const Point3D& p) const {
	return p;
}

#endif