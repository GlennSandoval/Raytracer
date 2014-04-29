#ifndef __IMGTEXTURE__
#define __IMGTEXTURE__

#include "Texture.h"
#include "../Utilities/ShadeRec.h"
#include "../Mapping/Mapping.h"
#include "../Utilities/CImg.h"

class ImageTexture : public Texture {
	public:
		ImageTexture();
		ImageTexture(const char* filename);
		virtual Color get_color(const ShadeRec& sr) const;
		void set_mapping(Mapping* mp);
		void set_texture(const char* filename);
		int get_width() const;
		int get_height() const;
	private:
		cimg_library::CImg<unsigned char> texture;
		int hres;
		int vres;
		Mapping* mapping_ptr;
};

inline int ImageTexture::get_width() const {
	return hres;
}

inline int ImageTexture::get_height() const {
	return vres;
}

inline void ImageTexture::set_mapping(Mapping* mp) {
	mapping_ptr = mp;
}

inline void ImageTexture::set_texture(const char* filename) {
	texture = cimg_library::CImg<unsigned char>(filename);
	hres = texture.width();
	vres = texture.height();
}

#endif