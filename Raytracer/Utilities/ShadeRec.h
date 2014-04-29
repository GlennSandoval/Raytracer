#ifndef __SHADEREC__
#define __SHADEREC__

#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"
#include "Color.h"
//#include "../Material/Material.h"
//#include "../World/World.h"

class World;
class Material;

class ShadeRec {
	public: 
		bool hit_obj;
		Material* material_ptr;
		Point3D ph;
		Point3D local_ph;
		Normal nh;
		Ray ray;
		Color color;
		const World& world;
		int depth;
		float t;
		float u;
		float v;

		bool accumulate;

		ShadeRec(const World& w);
		ShadeRec(const ShadeRec& sr);
		~ShadeRec();

		ShadeRec& operator= (const ShadeRec& rhs);
};

#endif