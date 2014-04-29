#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include "../Texture/Texture.h"
#include "../Utilities/ShadeRec.h"

class Material {
	public:
		Material();

		virtual Color shade(ShadeRec& sr);
		virtual Color get_Le(const ShadeRec& sr) const;
		void set_texture(Texture* tp);
		void set_normalmap(Texture* nm);
		void set_name(std::string s);
		std::string get_name() const;

	protected:
		Texture* texture_ptr;
		Texture* normalmap_ptr;
		std::string name;
};

inline void Material::set_texture(Texture* tp) {
	texture_ptr = tp;
}

inline void Material::set_normalmap(Texture* nm) {
	normalmap_ptr = nm;
}

inline void Material::set_name(std::string s) {
	name = s;
}

inline std::string Material::get_name() const {
	return name;
}

#endif