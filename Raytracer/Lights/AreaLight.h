#ifndef __AREALIGHT__
#define __AREALIGHT__

#include "../Geometry/GeometricObject.h"
#include "Light.h"
#include "../Material/Material.h"

class AreaLight : public Light {
	public:
		AreaLight();

		void set_object(GeometricObject* obj_ptr);

		virtual Vector3D get_direction(const ShadeRec& sr);
		virtual Color L(const ShadeRec& sr);

		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);

	private:
		GeometricObject* object_ptr;
		Material* material_ptr;
		Point3D sample_point;
		Normal normal;
		Vector3D wi;
};

inline void AreaLight::set_object(GeometricObject* obj_ptr) {
	object_ptr = obj_ptr;
	material_ptr = object_ptr->get_material();
}

#endif