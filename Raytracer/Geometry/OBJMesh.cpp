#include "OBJMesh.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>

OBJMesh::OBJMesh()
{}

OBJMesh::OBJMesh(const char* filename, float scale, Point3D translate) {
	std::ifstream fin(filename, std::ios::in);

	if (fin.is_open()) {
		std::string line;
		while (std::getline(fin, line)) {
			std::istringstream iss(line);

			std::vector<std::string> tokens;
			std::string token;
			while (iss >> token) {
				tokens.push_back(token);
			}

			if (tokens.size() > 0) {
				if (tokens[0] == "v") {
					Point3D* p = new Point3D((scale * std::stof(tokens[1].c_str())) + translate.x, (scale * std::stof(tokens[2].c_str())) + translate.y, (scale * std::stof(tokens[3].c_str())) + translate.z);
					points.push_back(p);
				}
				/* sometimes obj files have vertex normals, but we want to ignore them and just do our own interpolation
				else if (tokens[0] == "vn") {
					Normal n = Normal(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
					normals.push_back(n);
				}*/
				else if (tokens[0] == "vt") {
					u.push_back(std::stof(tokens[1].c_str()));
					v.push_back(std::stof(tokens[2].c_str()));
				}
				else if (tokens[0] == "f") {
					if (point_faces.size() == 0)
						point_faces.resize(points.size());

					// format is vertex/texture/normal
					std::vector<std::string> first = split(tokens[1], '/');
					std::vector<std::string> second = split(tokens[2], '/');
					std::vector<std::string> third = split(tokens[3], '/');

					int p0 = (int)atof(first[0].c_str())-1;
					int p1 = (int)atof(second[0].c_str())-1;
					int p2 = (int)atof(third[0].c_str())-1;

					Triangle* t = new Triangle(points[p0], points[p1], points[p2]);

					point_faces[p0].push_back(t);
					point_faces[p1].push_back(t);
					point_faces[p2].push_back(t);

					if (first.size() > 1) {
						int uv0 = (int)atof(first[1].c_str())-1;
						int uv1 = (int)atof(second[1].c_str())-1;
						int uv2 = (int)atof(third[1].c_str())-1;
						t->set_tex_coords(Point2D(u[uv0], v[uv0]), Point2D(u[uv1], v[uv1]), Point2D(u[uv2], v[uv2]));
					}
					
					faces.push_back(t);
				}
			}
		}
	}

	std::cout << points.size() << " vertices" << std::endl;
	std::cout << faces.size() << " faces" << std::endl;

	fin.close();

	for (unsigned int i=0; i < points.size(); i++) { // for each point
		Normal norm = Vector3D(0,0,0);
		for (unsigned int j=0; j < point_faces[i].size(); j++) { // for each triangle
			norm.x += point_faces[i][j]->get_normal().x; // aggregate normals of all faces
			norm.y += point_faces[i][j]->get_normal().y;
			norm.z += point_faces[i][j]->get_normal().z;
		}
		norm.normalize();
		points[i]->normal = norm;
	}

	kdtree = new KDNode(faces);
}

bool OBJMesh::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	return kdtree->hit(kdtree, ray, t, tmin, sr);
	
	/* 
		* the below code will hit every face individually
		* it is much faster to use the KD tree instead
		*/
	/*float t;
	Normal normal;
	Point3D hit_pt, local_hit_pt;
	for (int i=0; i < faces.size(); i++) {
		if (faces[i]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_obj = true;
			tmin = t;
			sr.ph = ray.o + t * ray.d;
			hit_pt = sr.ph;
			sr.nh = faces[i]->get_normal();
			normal = sr.nh;
			sr.local_ph = faces[i]->translate_to_local_coords(sr.ph);
			local_hit_pt = sr.local_ph;
		}
	}
	if (sr.hit_obj) {
		sr.t = tmin;
		sr.nh = normal;
		sr.ph = hit_pt;
		sr.local_ph = local_hit_pt;
		return true;
	}
	return false;*/
}

bool OBJMesh::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return kdtree->shadow_hit(kdtree, ray, tmin, Li);
	
	/*
		* Code below will calculate hits for every face individually, rather
		* than using the KD tree, which is way way faster
		*/
	/*float t;
	bool shadow_hit = false;
	for (int i=0; i < faces.size(); i++) {
		if (faces[i]->shadow_hit(ray, t, Li) && (t < tmin)) {
			tmin = t;
			shadow_hit = true;
		}
	}
	if (shadow_hit) return true;
	return false;*/
}

std::vector<std::string> OBJMesh::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while(getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
