#include "Geodesic.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>

Geodesic::Geodesic()
{}

Geodesic::Geodesic(const char* filename) {
	std::ifstream fin(filename, std::ios::in);

	if (fin.is_open()) {
		std::string line;
		while (getline(fin, line)) {
			std::istringstream iss(line);

			std::vector<std::string> tokens;
			std::string token;
			while (iss >> token) {
				tokens.push_back(token);
			}

			if (tokens.size() > 0) {
				if (tokens[0] == "v") {
					Point3D* p = new Point3D(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
					points.push_back(p);
				}
			}
		}
	}

	std::cout << points.size() << " vertices" << std::endl;

	fin.close();
}

std::vector<std::string> Geodesic::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

bool Geodesic::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	return false;
}
bool Geodesic::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return false;
}
