#include <iostream>
#include <sstream>
#include <algorithm>

#include "../Utilities/tinyxml2.h"

#include "ErrorStrings.h"

#include "Scene.h"
#include "../Utilities/Constants.h"
#include "../Geometry/Plane.h"
#include "../Geometry/Sphere.h"
#include "../Geometry/Rect.h"
#include "../Geometry/OBJMesh.h"
#include "../Geometry/VolumetricSphere.h"
#include "../Simulations/fire.h"
#include "../Geometry/VolumetricFire.h"

// lighting
#include "../Lights/PointLight.h"
#include "../Lights/DirectionalLight.h"
#include "../Lights/Spotlight.h"
#include "../Lights/AreaLight.h"
#include "../Lights/AmbientOcclusion.h"
#include "../Lights/EnvironmentLighting.h"

// materials
#include "../Material/Lambert.h"
#include "../Material/Gooch.h"
#include "../Material/Toon.h"
#include "../Material/Phong.h"
#include "../Material/Emissive.h"
#include "../Material/Reflective.h"
#include "../Material/GlossyReflective.h"
#include "../Material/Refractive.h"
#include "../Material/Translucent.h"
#include "../Material/Iridescent.h"

// utilities
#include "../Utilities/Point2D.h"
#include "../Utilities/Vector3D.h"
#include "../Utilities/Point3D.h"
#include "../Utilities/Normal.h"
#include "../Utilities/ShadeRec.h"

#include "../Tracers/RayCast.h"
#include "../Samplers/Jittered.h"

// mapping and textures
#include "../Mapping/SphericalMapping.h"
#include "../Mapping/PlanarMapping.h"
#include "../Texture/ImageTexture.h"
#include "../Texture/CheckerTexture.h"
#include "../Texture/JuliaTexture.h"

Scene::Scene() {}

bool Scene::readSceneFile(const char* filename, World* world) {
	cout << "Building scene..." << endl;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename))
	{
		printf("load file=[%s] failed\n", filename);
		cout << "Invalid XML file. Check file for errors." << endl;
		return false;
	}
	tinyxml2::XMLHandle docHandle( &doc );
	tinyxml2::XMLElement* root = docHandle.FirstChildElement("scene").ToElement();
	if (!root) return missingElementError(ROOT_XML_ERROR);

	// set up camera and environment stuff
	if (!readCamera(root, world)) {
		return false;
	}

	// set up lights
	if (!readLights(root, world)) {
		return false;
	}

	// set up materials
	if (!readMaterials(root, world)) {
		return false;
	}
    
	// set up objects
	if (!readObjects(root, world)) {
		return false;
	}

	cout << "Scene built successfully." << endl;
	return true;
}

bool Scene::missingElementError(const char * error) const {
	cout << error << endl;
	return false;
}

bool Scene::readCamera(tinyxml2::XMLElement* root, World* world) {
	/* 
		* CAMERA
		*/
	// make sure all the required camera elements are there
	tinyxml2::XMLElement* cam = root->FirstChildElement("camera");
	if (!cam) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* origin_elem = cam->FirstChildElement("origin");
	if (!origin_elem) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* dims_elem = cam->FirstChildElement("dimensions");
	if (!dims_elem) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* aspect_elem = cam->FirstChildElement("aspect");
	if (!aspect_elem) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* d_elem = cam->FirstChildElement("d");
	if (!d_elem) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* f_elem = cam->FirstChildElement("focal");
	if (!f_elem) return missingElementError(CAMERA_XML_ERROR);
	tinyxml2::XMLElement* lr_elem = cam->FirstChildElement("lens_radius");
	if (!lr_elem) return missingElementError(CAMERA_XML_ERROR);

	// location
	const char* origin = origin_elem->GetText();
	istringstream iss;
	iss.str(origin);
	Point3D p = Point3D();
	iss >> p.x >> p.y >> p.z;
	iss.str("");
	iss.clear();
	// image dimensions
	const char* dims = dims_elem->GetText();
	iss.str(dims);
	int hres, vres;
	iss >> hres >> vres;
	iss.str("");
	iss.clear();
	// aspect ratio
	const char* aspect = aspect_elem->GetText();
	iss.str(aspect);
	int sx, sy;
	iss >> sx >> sy;
	iss.str("");
	iss.clear();
	// d
	int d = atoi(d_elem->GetText());
	// focal length
	float f = stof(f_elem->GetText());
	// lens radius
	float lr = stof(lr_elem->GetText());
	// build camera object
	world->set_camera(Camera(p, d, hres, vres, sx, sy, f, lr));

	/*
	* SAMPLER
	*/
	// make sure all the required elements are there
	tinyxml2::XMLElement* sampler = root->FirstChildElement("sampler");
	if (!sampler) return missingElementError(SAMPLER_XML_ERROR);
	tinyxml2::XMLElement* sampler_type = sampler->FirstChildElement("type");
	if (!sampler_type) return missingElementError(SAMPLER_XML_ERROR);
	tinyxml2::XMLElement* num_samples = sampler->FirstChildElement("samples");
	if (!num_samples) return missingElementError(SAMPLER_XML_ERROR);

	string type = sampler_type->GetText();
	int samples = atoi(num_samples->GetText());
	transform(type.begin(), type.end(), type.begin(), ::tolower);
	if (type == "jittered") {
		world->set_sampler(new Jittered(samples));
	}
	else {
		cout << "Invalid sampler type" << endl;
		cout << "Valid sampler types: jittered" << endl;
		return false;
	}

	/*
	* Max recursion depth
	*/
	tinyxml2::XMLElement* max_depth_elem = root->FirstChildElement("recursion_depth");
	if (!max_depth_elem) return missingElementError(RECURSION_DEPTH_XML_ERROR);

	world->set_max_depth(atoi(max_depth_elem->GetText()));

	/*
	* Size of squares threads will render at a time
	*/
	tinyxml2::XMLElement* chunk_size_elem = root->FirstChildElement("chunk_size");
	if (!chunk_size_elem) return missingElementError(CHUNK_SIZE_XML_ERROR);

	world->set_chunksize(atoi(chunk_size_elem->GetText()));
	
	world->set_image(new cimg_library::CImg<unsigned char>(hres, vres, 1, 3, 0));

	return true;
}

bool Scene::readLights(tinyxml2::XMLElement* root, World* world) {
	istringstream iss;
	Point3D p = Point3D();
	Color c;

	/*
	* Environment map
	*/
	// make sure we have all the required elements
	tinyxml2::XMLElement* lighting_elem = root->FirstChildElement("lighting");
	if (!lighting_elem) return missingElementError(LIGHTING_XML_ERROR);
	tinyxml2::XMLElement* env_map_elem = lighting_elem->FirstChildElement("environment_map");
	if (!env_map_elem) return missingElementError(LIGHTING_XML_ERROR);
	tinyxml2::XMLElement* bgcolor_elem = lighting_elem->FirstChildElement("bgcolor");
	if (!bgcolor_elem) return missingElementError(LIGHTING_XML_ERROR);

	const char* env_map_file = env_map_elem->GetText();
	ImageTexture* env_map = new ImageTexture(env_map_file);
	SphericalMapping *spherical_map_ptr = new SphericalMapping;
	env_map->set_mapping(spherical_map_ptr);
	world->set_env_map(env_map);

	const char* color = bgcolor_elem->GetText();
	iss.str(color);
	iss >> c.r >> c.g >> c.b;
	iss.str("");
	iss.clear();
	world->set_bgcolor(c);
	
	/*
	* LIGHTS
	*/
	// make sure we have all the required elements
	tinyxml2::XMLElement* lights = lighting_elem->FirstChildElement("lights");
	if (!lights) return missingElementError(LIGHTING_XML_ERROR);
	tinyxml2::XMLElement* light = lights->FirstChildElement("light");
	if (!light) return missingElementError(LIGHTING_XML_ERROR);

	for (tinyxml2::XMLElement* e = light; e != NULL; e = e->NextSiblingElement("light")) {
		// make sure the light has all the required elements
		tinyxml2::XMLElement* shadows_elem = e->FirstChildElement("casts_shadows");
		if (!shadows_elem) return missingElementError(LIGHTING_XML_ERROR);
		tinyxml2::XMLElement* type_elem = e->FirstChildElement("type");
		if (!type_elem) return missingElementError(LIGHTING_XML_ERROR);

		// shadows
		const char* shadows = shadows_elem->GetText();
		bool casts_shadows = !strcmp(shadows,"true") ? true : false;
		// type
		string type = type_elem->GetText();
		if (type == "point") {
			tinyxml2::XMLElement* color_elem = e->FirstChildElement("color");
			if (!color_elem) return missingElementError(POINT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* rad_elem = e->FirstChildElement("radiance");
			if (!rad_elem) return missingElementError(POINT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* loc_elem = e->FirstChildElement("location");
			if (!loc_elem) return missingElementError(POINT_LIGHT_XML_ERROR);

			// color
			const char* color = color_elem->GetText();
			iss.str(color);
			iss >> c.r >> c.g >> c.b;
			iss.str("");
			iss.clear();
			// radiance
			float radiance = stof(rad_elem->GetText());
			// location
			const char* location = loc_elem->GetText();
			iss.str(location);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			// create the light
			PointLight* light_ptr = new PointLight;
			light_ptr->set_color(c);
			light_ptr->set_radiance(radiance);
			light_ptr->set_location(p);
			light_ptr->set_shadows(casts_shadows);
			world->add_light(light_ptr);
		}
		else if (type == "directional") {
			tinyxml2::XMLElement* color_elem = e->FirstChildElement("color");
			if (!color_elem) return missingElementError(DIRECTIONAL_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* rad_elem = e->FirstChildElement("radiance");
			if (!rad_elem) return missingElementError(DIRECTIONAL_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* dir_elem = e->FirstChildElement("direction");
			if (!dir_elem) return missingElementError(DIRECTIONAL_LIGHT_XML_ERROR);

			// color
			const char* color = color_elem->GetText();
			iss.str(color);
			iss >> c.r >> c.g >> c.b;
			iss.str("");
			iss.clear();
			// radiance
			float radiance = stof(rad_elem->GetText());
			// direction
			const char* direction = dir_elem->GetText();
			iss.str(direction);
			Vector3D dir;
			iss >> dir.x >> dir.y >> dir.z;
			iss.str("");
			iss.clear();
			// create the light
			DirectionalLight* light_ptr = new DirectionalLight;
			light_ptr->set_color(c);
			light_ptr->set_radiance(radiance);
			light_ptr->set_direction(dir);
			light_ptr->set_shadows(casts_shadows);
			world->add_light(light_ptr);
		}
		else if (type == "spot") {
			tinyxml2::XMLElement* color_elem = e->FirstChildElement("color");
			if (!color_elem) return missingElementError(SPOT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* rad_elem = e->FirstChildElement("radiance");
			if (!rad_elem) return missingElementError(SPOT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* loc_elem = e->FirstChildElement("location");
			if (!loc_elem) return missingElementError(SPOT_LIGHT_XML_ERROR);

			// color
			const char* color = color_elem->GetText();
			iss.str(color);
			iss >> c.r >> c.g >> c.b;
			iss.str("");
			iss.clear();
			// radiance
			float radiance = stof(rad_elem->GetText());
			// location
			const char* location = loc_elem->GetText();
			iss.str(location);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			// create the light
			Spotlight* light_ptr = new Spotlight;
			light_ptr->set_color(c);
			light_ptr->set_radiance(radiance);
			light_ptr->set_location(p);
			light_ptr->set_shadows(casts_shadows);
			// TODO: add texture?
			world->add_light(light_ptr);
		}
		else if (type == "area") {
			// for rectangular area light
			tinyxml2::XMLElement* color_elem = e->FirstChildElement("color");
			if (!color_elem) return missingElementError(SPOT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* rad_elem = e->FirstChildElement("radiance");
			if (!rad_elem) return missingElementError(SPOT_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* corner_elem = e->FirstChildElement("corner");
			if (!corner_elem) return missingElementError(AREA_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* side1_elem = e->FirstChildElement("side1");
			if (!side1_elem) return missingElementError(AREA_LIGHT_XML_ERROR);
			tinyxml2::XMLElement* side2_elem = e->FirstChildElement("side2");
			if (!side2_elem) return missingElementError(AREA_LIGHT_XML_ERROR);

			// color
			const char* color = color_elem->GetText();
			iss.str(color);
			iss >> c.r >> c.g >> c.b;
			iss.str("");
			iss.clear();
			// radiance
			float radiance = stof(rad_elem->GetText());
			// corner
			const char* corner = corner_elem->GetText();
			iss.str(corner);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			// 2 side vectors
			const char* side1 = side1_elem->GetText();
			const char* side2 = side2_elem->GetText();
			Vector3D v1;
			Vector3D v2;
			iss.str(side1);
			iss >> v1.x >> v1.y >> v1.z;
			iss.str("");
			iss.clear();
			iss.str(side2);
			iss >> v2.x >> v2.y >> v2.z;
			iss.str("");
			iss.clear();

			// create emissive object, rectangle, and light
			Emissive* emissive_ptr = new Emissive;
			emissive_ptr->scale_radiance(radiance);
			emissive_ptr->set_color(c);

			Rect* rectangle_ptr = new Rect(p, v1, v2);
			rectangle_ptr->set_material(emissive_ptr);
			rectangle_ptr->set_sampler(world->get_sampler());
			world->add_object(rectangle_ptr);

			AreaLight* light_ptr = new AreaLight;
			light_ptr->set_object(rectangle_ptr);
			light_ptr->set_shadows(casts_shadows);
			world->add_light(light_ptr);
		}
		else if (type == "ambientocclusion") {
			AmbientOcclusion* light_ptr = new AmbientOcclusion;
			light_ptr->set_shadows(casts_shadows);
			world->add_light(light_ptr);
		}
		else if (type == "environment") {
			tinyxml2::XMLElement* file_elem = e->FirstChildElement("filename");
			if (!file_elem) return missingElementError(ENV_LIGHTING_XML_ERROR);

			const char* f = file_elem->GetText();
			EnvironmentLighting* light_ptr = new EnvironmentLighting(f);
			light_ptr->set_shadows(casts_shadows);
			world->add_light(light_ptr);
		}
		else {
			cout << "Invalid light type " << type << endl;
			cout << "Valid light types: point, directional, spot, area, ambientocclusion, environment" << endl;
			return false;
		}
	}

	return true;
}

bool Scene::readMaterials(tinyxml2::XMLElement* root, World* world) {
	istringstream iss;
	Point3D p = Point3D();
	Color c;
	
	/*
	* MATERIALS
	*/
	tinyxml2::XMLElement* materials_elem = root->FirstChildElement("materials");
	if (!materials_elem) return missingElementError(MATERIALS_XML_ERROR);
	tinyxml2::XMLElement* material = materials_elem->FirstChildElement("material");
	if (!material) return missingElementError(MATERIALS_XML_ERROR);

	for (tinyxml2::XMLElement* e = material; e != NULL; e = e->NextSiblingElement("material")) {
		tinyxml2::XMLElement* name_elem = e->FirstChildElement("name");
		if (!name_elem) return missingElementError(MATERIALS_XML_ERROR);
		tinyxml2::XMLElement* type_elem = e->FirstChildElement("type");
		if (!type_elem) return missingElementError(MATERIALS_XML_ERROR);
		
		// material name
		const char* name = name_elem->GetText();
		string mat_name = name;
		// check for texture and normal map
		bool hasTexture = false;
		bool hasNormalMap = false;
		tinyxml2::XMLElement* texture = e->FirstChildElement("texture");
		PlanarMapping* planar_map_ptr = new PlanarMapping;
		ImageTexture *img_tex_ptr = new ImageTexture;
		if (texture) {
			const char* tex = texture->GetText();
			img_tex_ptr->set_texture(tex);
			img_tex_ptr->set_mapping(planar_map_ptr);
			hasTexture = true;
		}
		tinyxml2::XMLElement* nm = e->FirstChildElement("normal_map");
		ImageTexture *img_tex_ptr2 = new ImageTexture;
		if (nm) {
			const char* normalmap = nm->GetText();
			img_tex_ptr2->set_texture(normalmap);
			img_tex_ptr2->set_mapping(planar_map_ptr);
			hasNormalMap = true;
		}
		// check for color
		bool hasColor = false;
		tinyxml2::XMLElement* col = e->FirstChildElement("color");
		if (col) {
			const char* color = col->GetText();
			iss.str(color);
			iss >> c.r >> c.g >> c.b;
			iss.str("");
			iss.clear();
			hasColor = true;
		}
		// check type
		string type = type_elem->GetText();
		if (type == "lambert") {
			Lambert* lambert_ptr = new Lambert;
			lambert_ptr->set_name(name);
			if (hasColor) lambert_ptr->set_color(c);
			if (hasTexture) lambert_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) lambert_ptr->set_normalmap(img_tex_ptr2);
			add_material(lambert_ptr);
		}
		else if (type == "phong") {
			tinyxml2::XMLElement* spec_elem = e->FirstChildElement("specCoeff");
			if (!spec_elem) return missingElementError(PHONG_MATERIAL_XML_ERROR);

			float specCoeff = stof(spec_elem->GetText());
			Phong* phong_ptr = new Phong;
			phong_ptr->set_name(name);
			if (hasColor) phong_ptr->set_color(c);
			if (hasTexture) phong_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) phong_ptr->set_normalmap(img_tex_ptr2);
			phong_ptr->set_specCoeff(specCoeff);
			add_material(phong_ptr);
		}
		else if (type == "glossyreflective") {
			tinyxml2::XMLElement* spec_elem = e->FirstChildElement("specCoeff");
			if (!spec_elem) return missingElementError(GLOSSYREFLECTIVE_MATERIAL_XML_ERROR);
			tinyxml2::XMLElement* refl_elem = e->FirstChildElement("reflectivity");
			if (!refl_elem) return missingElementError(GLOSSYREFLECTIVE_MATERIAL_XML_ERROR);

			float specCoeff = stof(spec_elem->GetText());
			float reflectivity = stof(refl_elem->GetText());            
			GlossyReflective* glossrefl_ptr = new GlossyReflective;
			glossrefl_ptr->set_name(name);
			if (hasColor) glossrefl_ptr->set_color(c);
			if (hasTexture) glossrefl_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) glossrefl_ptr->set_normalmap(img_tex_ptr2);
			glossrefl_ptr->set_specCoeff(specCoeff);
			glossrefl_ptr->reflectivity = reflectivity;
			add_material(glossrefl_ptr);
		}
		else if (type == "translucent") {
			tinyxml2::XMLElement* spec_elem = e->FirstChildElement("specCoeff");
			if (!spec_elem) return missingElementError(TRANSLUCENT_MATERIAL_XML_ERROR);
			tinyxml2::XMLElement* eta_elem = e->FirstChildElement("eta");
			if (!eta_elem) return missingElementError(TRANSLUCENT_MATERIAL_XML_ERROR);

			float specCoeff = stof(spec_elem->GetText());
			float eta = stof(eta_elem->GetText());            
			Translucent* trans_ptr = new Translucent;
			trans_ptr->set_name(name);
			if (hasColor) trans_ptr->set_color(c);
			if (hasTexture) trans_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) trans_ptr->set_normalmap(img_tex_ptr2);
			trans_ptr->set_specCoeff(specCoeff);
			trans_ptr->eta = eta;
			add_material(trans_ptr);
		}
		else if (type == "reflective") {
			tinyxml2::XMLElement* spec_elem = e->FirstChildElement("specCoeff");
			if (!spec_elem) return missingElementError(REFLECTIVE_MATERIAL_XML_ERROR);
			tinyxml2::XMLElement* refl_elem = e->FirstChildElement("reflectivity");
			if (!refl_elem) return missingElementError(REFLECTIVE_MATERIAL_XML_ERROR);

			float specCoeff = stof(spec_elem->GetText());
			float reflectivity = stof(refl_elem->GetText());
			Reflective* refl_ptr = new Reflective;
			refl_ptr->set_name(name);
			if (hasColor) refl_ptr->set_color(c);
			if (hasTexture) refl_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) refl_ptr->set_normalmap(img_tex_ptr2);
			refl_ptr->set_specCoeff(specCoeff);
			refl_ptr->reflectivity = reflectivity;
			add_material(refl_ptr);
		}
		else if (type == "refractive") {
			tinyxml2::XMLElement* spec_elem = e->FirstChildElement("specCoeff");
			if (!spec_elem) return missingElementError(REFRACTIVE_MATERIAL_XML_ERROR);
			tinyxml2::XMLElement* trans_elem = e->FirstChildElement("transmission");
			if (!trans_elem) return missingElementError(REFRACTIVE_MATERIAL_XML_ERROR);
			tinyxml2::XMLElement* eta_elem = e->FirstChildElement("eta");
			if (!eta_elem) return missingElementError(REFRACTIVE_MATERIAL_XML_ERROR);

			float specCoeff = stof(spec_elem->GetText());
			float transmission = stof(trans_elem->GetText());
			float eta = stof(eta_elem->GetText());
			Refractive* refr_ptr = new Refractive;
			refr_ptr->set_name(name);
			if (hasColor) refr_ptr->set_color(c);
			if (hasTexture) refr_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) refr_ptr->set_normalmap(img_tex_ptr2);
			refr_ptr->set_specCoeff(specCoeff);
			refr_ptr->transmission = transmission;
			refr_ptr->eta = eta;
			add_material(refr_ptr);
		}
		else if (type == "iridescent") {
			tinyxml2::XMLElement* eta_elem = e->FirstChildElement("eta");
			if (!eta_elem) return missingElementError(IRIDESCENT_MATERIAL_XML_ERROR);

			float eta = stof(eta_elem->GetText());
			Iridescent* iridescent_ptr = new Iridescent;
			iridescent_ptr->set_name(name);
			if (hasTexture) iridescent_ptr->set_texture(img_tex_ptr);
			if (hasNormalMap) iridescent_ptr->set_normalmap(img_tex_ptr2);
			iridescent_ptr->eta = eta;
			add_material(iridescent_ptr);
		}
		else {
			cout << "Invalid material type " << type << endl;
			cout << "Valid material types: lambert, phong, glossyreflective, translucent, reflective, refractive, iridescent" << endl;
			return false;
		}
	}
	return true;
}

bool Scene::readObjects(tinyxml2::XMLElement* root, World* world) {
	istringstream iss;
	Point3D p = Point3D();
	Color c;
	
	/*
	* OBJECTS
	*/
	tinyxml2::XMLElement* objects = root->FirstChildElement("objects");
	if (!objects) return missingElementError(OBJECTS_XML_ERROR);
	tinyxml2::XMLElement* obj = objects->FirstChildElement("object");
	if (!obj) return missingElementError(OBJECTS_XML_ERROR);

	for (tinyxml2::XMLElement* e = obj; e != NULL; e = e->NextSiblingElement("object")) {
		tinyxml2::XMLElement* mat_elem = e->FirstChildElement("material");
		if (!mat_elem) return missingElementError(OBJECTS_XML_ERROR);
		tinyxml2::XMLElement* type_elem = e->FirstChildElement("type");
		if (!type_elem) return missingElementError(OBJECTS_XML_ERROR);

		const char* mat = mat_elem->GetText();
		Material* mat_ptr = new Material;
		for (unsigned int i=0; i<materials.size(); i++) {
			if (materials[i]->get_name() == mat) {
				mat_ptr = materials[i];
			}
		}
		string type = type_elem->GetText();
		if (type == "plane") {
			tinyxml2::XMLElement* center_elem = e->FirstChildElement("center");
			if (!center_elem) return missingElementError(PLANE_XML_ERROR);
			tinyxml2::XMLElement* norm_elem = e->FirstChildElement("normal");
			if (!norm_elem) return missingElementError(PLANE_XML_ERROR);

			const char* center = center_elem->GetText();
			iss.str(center);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			const char* normal = norm_elem->GetText();
			iss.str(normal);
			Normal n;
			iss >> n.x >> n.y >> n.z;
			iss.str("");
			iss.clear();
			Plane* plane_ptr = new Plane(p, n);
			plane_ptr->set_material(mat_ptr);
			world->add_object(plane_ptr);
		}
		else if (type == "sphere") {
			tinyxml2::XMLElement* center_elem = e->FirstChildElement("center");
			if (!center_elem) return missingElementError(SPHERE_XML_ERROR);
			tinyxml2::XMLElement* radius_elem = e->FirstChildElement("radius");
			if (!radius_elem) return missingElementError(SPHERE_XML_ERROR);

			const char* center = center_elem->GetText();
			iss.str(center);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			float radius = stof(radius_elem->GetText());
			Sphere* sphere_ptr = new Sphere(p, radius);
			sphere_ptr->set_material(mat_ptr);
			world->add_object(sphere_ptr);
		}
		else if (type == "OBJ") {
			tinyxml2::XMLElement* file_elem = e->FirstChildElement("filename");
			if (!file_elem) return missingElementError(OBJ_XML_ERROR);
			tinyxml2::XMLElement* loc_elem = e->FirstChildElement("location");
			if (!loc_elem) return missingElementError(OBJ_XML_ERROR);
			tinyxml2::XMLElement* scale_elem = e->FirstChildElement("scale");
			if (!scale_elem) return missingElementError(OBJ_XML_ERROR);

			const char* filename = file_elem->GetText();
			const char* location = loc_elem->GetText();
			iss.str(location);
			iss >> p.x >> p.y >> p.z;
			iss.str("");
			iss.clear();
			float scale = stof(scale_elem->GetText());
			OBJMesh* objmesh_ptr = new OBJMesh(filename, scale, p);
			objmesh_ptr->set_material(mat_ptr);
			world->add_object(objmesh_ptr);
		}
		else {
			cout << "Invalid object type " << type << endl;
			cout << "Valid object types: plane, sphere, OBJ" << endl;
			return false;
		}
	}
	return true;
}
