#ifndef __WORLD__
#define __WORLD__

#include <vector>

#include "../Camera/Camera.h"
#include "../Utilities/Color.h"
#include "../Samplers/Sampler.h"
#include "../Geometry/GeometricObject.h"
#include "../Utilities/Ray.h"

#include "../Lights/Light.h"
#include "../Texture/ImageTexture.h"
#include "../Utilities/CImg.h"

class World {
	private:
		Camera camera;
		Color bgcolor;
		Sampler* sampler_ptr;
		std::vector<GeometricObject*> objects;
		std::vector<Light*> lights;
		cimg_library::CImg<unsigned char> *image;
		ImageTexture* env_map;
		int max_depth;
		int chunkSize;
	
	public:
		World();
		World(const World& world);
		
		bool build(const char* filename);

		Camera get_camera() const;
		Color get_bgcolor() const;
		Sampler* get_sampler() const;
		cimg_library::CImg<unsigned char>* get_image() const;
		ImageTexture* get_env_map() const;
		int get_max_depth() const;
		int get_chunk_size() const;
		GeometricObject* get_object(int index) const;
		Light* get_light(int index) const;
		int get_num_objects() const;
		int get_num_lights() const;

		void set_camera(Camera& camera);
		void set_bgcolor(Color& c);
		void set_sampler(Sampler* s);
		void set_image(cimg_library::CImg<unsigned char> *img);
		void set_env_map(ImageTexture* imgTex);
		void set_max_depth(int n);
		void set_chunksize(int n);

		void add_light(Light* light_ptr);
		void add_object(GeometricObject* object_ptr);
        
		void render(int chunk) const;
		void draw_pixel(const int col, const int row, const Color& c) const;
		Color trace_ray(const Ray& ray, int depth) const;
		ShadeRec hit_objects(const Ray& ray) const;
};

inline Camera World::get_camera() const {
	return camera;
}

inline Color World::get_bgcolor() const {
	return bgcolor;
}

inline Sampler* World::get_sampler() const {
	return sampler_ptr;
}

inline cimg_library::CImg<unsigned char>* World::get_image() const {
	return image;
}

inline ImageTexture* World::get_env_map() const {
	return env_map;
}

inline int World::get_max_depth() const {
	return max_depth;
}

inline int World::get_chunk_size() const {
	return chunkSize;
}

inline GeometricObject* World::get_object(int index) const {
	return objects[index];
}

inline Light* World::get_light(int index) const {
	return lights[index];
}

inline int World::get_num_objects() const {
	return objects.size();
}

inline int World::get_num_lights() const {
	return lights.size();
}

inline void World::set_camera(Camera& cam) {
	camera = cam;
}

inline void World::set_bgcolor(Color& c) {
	bgcolor = c;
}

inline void World::set_sampler(Sampler* s) {
	sampler_ptr = s;
}

inline void World::set_image(cimg_library::CImg<unsigned char> *img) {
	image = img;
}

inline void World::set_env_map(ImageTexture* imgTex) {
	env_map = imgTex;
}

inline void World::set_max_depth(int n) {
	max_depth = n;
}

inline void World::set_chunksize(int n) {
	chunkSize = n;
}

inline void World::add_object(GeometricObject* object_ptr) {
	objects.push_back(object_ptr);
}

inline void World::add_light(Light* light_ptr) {
	lights.push_back(light_ptr);
}

#endif