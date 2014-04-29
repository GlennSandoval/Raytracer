#ifndef __JULIATEXTURE__
#define __JULIATEXTURE__

#include "Texture.h"
#include "../Utilities/ShadeRec.h"
#include "../Mapping/Mapping.h"
#include <vector>

class JuliaTexture : public Texture {
	public:
		JuliaTexture();
		virtual Color get_color(const ShadeRec& sr) const;
		void set_mapping(Mapping* mp);

	protected:
		int size;
		std::vector<float> values;
		Mapping* mapping_ptr;
};

inline void JuliaTexture::set_mapping(Mapping* mp) {
	mapping_ptr = mp;
}

#endif