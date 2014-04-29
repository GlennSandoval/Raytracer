#include "KDTree.h"
#include <algorithm>
#include <iostream>

KDNode::KDNode() : triangles(std::vector<Triangle*>()), left(NULL), right(NULL), bbox(BoundingBox())
{}

KDNode::KDNode(const KDNode& node) : triangles(node.triangles), left(node.left), right(node.right), bbox(node.bbox)
{}

KDNode::KDNode(std::vector<Triangle*>& tris) : triangles(std::vector<Triangle*>()), left(NULL), right(NULL)
{
	KDNode* node = build(tris, 0);
	triangles = node->triangles;
	left = node->left;
	right = node->right;
	bbox = node->bbox;
}

KDNode* KDNode::build(std::vector<Triangle*>& tris, int depth) const{
	KDNode* node = new KDNode();
	node->triangles = tris;
	node->left = NULL;
	node->right = NULL;
	node->bbox = BoundingBox();
	
	if (tris.size() == 0) 
		return node;
	
	if (tris.size() == 1) {
		node->bbox = tris[0]->get_bounding_box();
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();
		return node;
	}

	// get a bounding box surrounding all the triangles
	node->bbox = tris[0]->get_bounding_box();

	for (unsigned int i=1; i < tris.size(); i++) {
		node->bbox.expand(tris[i]->get_bounding_box());
	}	
	
	Point3D midpt = Point3D(0,0,0);
	for (unsigned int i=0; i < tris.size(); i++) {
		// find midpoint of all triangles
		midpt = midpt + (tris[i]->get_midpoint() * (1.0f / (float)tris.size()));
	}

	std::vector<Triangle*> left_tris;
	std::vector<Triangle*> right_tris;
	int axis = node->bbox.longest_axis();
	for (unsigned int i=0; i < tris.size(); i++) {
		// split triangles based on their midpoints side of avg in longest axis
		switch (axis) {
			case 0:
				midpt.x >= tris[i]->get_midpoint().x ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
				break;
			case 1:
				midpt.y >= tris[i]->get_midpoint().y ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
				break;
			case 2:
				midpt.z >= tris[i]->get_midpoint().z ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
				break;
		}
	}

	if (left_tris.size() == 0 && right_tris.size() > 0) left_tris = right_tris;
	if (right_tris.size() == 0 && left_tris.size() > 0) right_tris = left_tris;

	// if 50% of triangles match, don't subdivide any more
	int matches = 0;
	for (unsigned int i=0; i < left_tris.size(); i++) {
		for (unsigned int j=0; j < right_tris.size(); j++) {
			if (left_tris[i] == right_tris[j])
				matches++;
		}
	}

	if ((float)matches / left_tris.size() < 0.5 && (float)matches / right_tris.size() < 0.5) {
		// recurse down left and right sides
		node->left = build(left_tris, depth + 1);
		node->right = build(right_tris, depth + 1);
	}
	else {
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();
	}

	return node;
}

KDNode& KDNode::operator=(const KDNode& rhs) {
	if (this == &rhs)
		return *this;

	bbox = rhs.bbox;
	left = rhs.left;
	right = rhs.right;
	for (unsigned int i=0; i < rhs.triangles.size(); i++) {
		triangles.push_back(rhs.triangles[i]);
	}

	return *this;
}

bool KDNode::hit(KDNode* node, const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {	
	if (node->bbox.hit(ray)) {
		Normal normal;
		bool hit_tri = false;
		Point3D hit_pt, local_hit_pt;

		if (node->left != NULL && node->left->triangles.size() > 0 || node->right != NULL && node->right->triangles.size() > 0) {
			bool hitleft = hit(node->left, ray, t, tmin, sr);
			bool hitright = hit(node->right, ray, t, tmin, sr);
			return hitleft || hitright;
		}
		else {
			for (unsigned int i=0; i < node->triangles.size(); i++) {
				if (node->triangles[i]->hit(ray, t, tmin, sr)) {
					hit_tri = true;
					sr.hit_obj = true;
					tmin = t;
					//sr.ph = ray.o + t * ray.d;
					hit_pt = sr.ph;
					//sr.nh = node->triangles[i]->get_normal();
					normal = sr.nh;
					sr.local_ph = node->triangles[i]->translate_to_local_coords(sr.ph);
					local_hit_pt = sr.local_ph;
				}
			}
			if (hit_tri) {
				sr.hit_obj = true;
				sr.t = tmin;
				sr.nh = normal;
				sr.ph = hit_pt;
				sr.local_ph = local_hit_pt;
				return true;
			}
			return false;
		}
	}
	return false;
}

bool KDNode::shadow_hit(KDNode* node, const Ray& ray, float& tmin, float Li) const {
	if (node->bbox.hit(ray)/* || bbox.inside(ray.o)*/) {
		float t;
		bool shadow_hit = false;

		if (node->left != NULL && node->left->triangles.size() > 0 || node->right != NULL && node->right->triangles.size() > 0) {
			bool hitleft = node->shadow_hit(node->left, ray, tmin, Li);
			bool hitright = node->shadow_hit(node->right, ray, tmin, Li);
			return hitleft || hitright;
		}
		else {
			for (unsigned int i=0; i < node->triangles.size(); i++) {
				if (node->triangles[i]->shadow_hit(ray, t, Li)) {
					shadow_hit = true;
					tmin = t;
				}
			}
			if (shadow_hit) {
				return true;
			}
			return false;
		}
	}
	return false;
}