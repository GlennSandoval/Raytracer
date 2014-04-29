#ifndef __SCENE__
#define __SCENE__

#include <vector>

#include "../Material/Material.h"
#include "../World/World.h"

class Scene {
	private:
		std::vector<Material*> materials;
	public:
		Scene();

		void add_material(Material* material_ptr);
		
		bool readSceneFile(const char* filename, World* world);
		bool readCamera(tinyxml2::XMLElement* root, World* world);
		bool readLights(tinyxml2::XMLElement* root, World* world);
		bool readMaterials(tinyxml2::XMLElement* root, World* world);
		bool readObjects(tinyxml2::XMLElement* root, World* world);
		bool missingElementError(const char * error) const;
};

inline void Scene::add_material(Material* material_ptr) {
	materials.push_back(material_ptr);
}

#endif