#ifndef __CHECKERTEXTURE__
#define __CHECKERTEXTURE__

#include "../Utilities/ShadeRec.h"
#include "Texture.h"
#include "../Mapping/Mapping.h"

class CheckerTexture : public Texture {
	public:
		CheckerTexture();
		CheckerTexture(Color c1, Color c2);
		virtual Color get_color(const ShadeRec& sr) const;
		void set_size(float s);

	protected:
		Color color1;
		Color color2;
		float size;
};

inline void CheckerTexture::set_size(float s) {
	size = s;
}

#endif