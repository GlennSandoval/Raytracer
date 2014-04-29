#include <iostream>
#include <sstream>
#include <algorithm>

#include "../Utilities/tinyxml2.h"

#include "../SceneFiles/Scene.h"
#include "../Utilities/Constants.h"
#include "../Material/Material.h"
#include "World.h"

World::World()
{}

bool World::build(const char* filename) {
	Scene scene = Scene();
	return scene.readSceneFile(filename, this);
}

void World::render(int chunk) const {
	Color pixColor;
	Ray ray;
	ray.o = camera.pe;
	Point2D sp;
	Point3D pp;
	Point2D lp;
	int n = sampler_ptr->get_num_samples();

	int hChunks = camera.hres / chunkSize;
	int vChunks = camera.vres / chunkSize;

	int row = chunk / hChunks;
	int col = chunk % hChunks;

	int vStart = row * chunkSize;
	int hStart = col * chunkSize;
	int vEnd = vStart+chunkSize > camera.vres ? camera.vres-1 : vStart+chunkSize;
	int hEnd = hStart+chunkSize > camera.hres ? camera.hres-1 : hStart+chunkSize;

	for (int r = vStart; r < vEnd; r++) {
		for (int c = hStart; c < hEnd; c++) {			
			pixColor = black;

			for (int i = 0; i < n*n; i++) {
				sp = sampler_ptr->sample_unit_square();
				float x = c + sp.x;
				float y = r + sp.y;

				float floor = -1.0, ceiling = 1.0, range = (ceiling - floor);
				float rnd1 = floor + (range * rand()) / (RAND_MAX + 1.0f);
				float rnd2 = floor + (range * rand()) / (RAND_MAX + 1.0f);

				lp = Point2D(rnd1, rnd2) * camera.lens_radius;

				pp = camera.p0 + (x / camera.hres) * camera.sx * camera.u + (y / camera.vres) * camera.sy * camera.v;

				ray.o = camera.pe + lp.x * camera.u + lp.y * camera.v;

				float d1 = (pp - camera.pe).mag();
				Point3D focalPlane = camera.pe + d1 * (camera.f / camera.d) * (pp - camera.pe).normalized();

				ray.d = (focalPlane - ray.o).normalized();

				pixColor += trace_ray(ray, 0);
			}

			pixColor /= (float)(n*n);
			draw_pixel(c, r, pixColor);
		}
	}
}

void World::draw_pixel(const int col, const int row, const Color& c) const {
	Color color = c;
	color.clamp();
	(*image)(col, row, 0, 0) = color.r * 255.0f;
	(*image)(col, row, 0, 1) = color.g * 255.0f;
	(*image)(col, row, 0, 2) = color.b * 255.0f;
}

Color World::trace_ray(const Ray& ray, int depth) const {
	if (depth > max_depth) {
		return black;
	}
	
	ShadeRec sr = hit_objects(ray);
	
	if (sr.hit_obj) {
		sr.depth = depth;
		sr.ray = ray;
		if (sr.accumulate)
			return sr.color;
		return sr.material_ptr->shade(sr);
	}
	else {
		if (env_map) {
			sr.local_ph.x = camera.u * ray.d.normalized();
			sr.local_ph.y = camera.v * ray.d.normalized();
			sr.local_ph.z = camera.w * ray.d.normalized();
			return env_map->get_color(sr);
		}
		else {
			return bgcolor;
		}
	}
}

ShadeRec World::hit_objects(const Ray& ray) const {
	ShadeRec sr(*this);
	float t;
	float tmin = LARGE_VAL;
	Normal normal;
	Material* material;
	Point3D hit_pt;
	Point3D local_hit_pt;
	int num_objs = objects.size();

	for (int j = 0; j < num_objs; j++) {
		if (objects[j]->hit(ray, t, tmin, sr)) {
			sr.hit_obj = true;
			tmin = t;
			sr.material_ptr = objects[j]->get_material();
			material = sr.material_ptr;
			hit_pt = sr.ph;
			sr.local_ph = objects[j]->translate_to_local_coords(sr.ph);
			local_hit_pt = sr.local_ph;
			normal = sr.nh;
		}
	}

	if (sr.hit_obj) {
		sr.t = tmin;
		sr.nh = normal;
		sr.ph = hit_pt;
		sr.local_ph = local_hit_pt;
		sr.material_ptr = material;
	}

	return sr;
}
