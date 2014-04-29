#ifndef __OBJMESH__
#define __OBJMESH__

#include "GeometricObject.h"
#include "Triangle.h"
#include "../Utilities/KDTree.h"
#include <vector>

class OBJMesh : public GeometricObject {
	public:
		std::vector<Triangle*> faces;
		KDNode* kdtree;

		OBJMesh();
		OBJMesh(const char* filename, float scale, Point3D translate);

		virtual bool hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin, float& Li) const;
		std::vector<std::string> split(const std::string &s, char delim);
		std::vector<Point3D*> get_points() const;

	private:
		std::vector<Point3D*> points;
		std::vector<std::vector<Triangle*>> point_faces;
		std::vector<Normal> normals;
		
		std::vector<float> u;
		std::vector<float> v;
};

inline std::vector<Point3D*> OBJMesh::get_points() const {
	return points;
}

#endif