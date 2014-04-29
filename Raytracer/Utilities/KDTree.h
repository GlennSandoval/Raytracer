#ifndef __KDTREE__
#define __KDTREE__

#include <vector>
#include "../Geometry/Triangle.h"

class KDNode {
	public:
		BoundingBox bbox;
		KDNode* left;
		KDNode* right;
		std::vector<Triangle*> triangles;

		KDNode();
		KDNode(const KDNode& node);
		KDNode(std::vector<Triangle*>& tris);

		KDNode* build(std::vector<Triangle*>& tris, int depth) const;

		KDNode& operator= (const KDNode& rhs);
		bool hit(KDNode* node, const Ray& ray, float& t, float& tmin, ShadeRec& sr) const;
		bool shadow_hit(KDNode* node, const Ray& ray, float& tmin, float Li) const;
};

#endif