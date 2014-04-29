#include "ImageTexture.h"
#include "../Utilities/Constants.h"

ImageTexture::ImageTexture() : mapping_ptr(NULL)
{}

ImageTexture::ImageTexture(const char* filename) : mapping_ptr(NULL)
{
	texture = cimg_library::CImg<unsigned char>(filename);
	hres = texture.width();
	vres = texture.height();
}

Color ImageTexture::get_color(const ShadeRec& sr) const {
	int row, col;

	if (mapping_ptr)
		mapping_ptr->get_pixel_coordinates(sr, hres, vres, row, col);
	else {
		row = (int) (sr.v * (vres - 1));
		col = (int) (sr.u * (hres - 1));
	}

	if (col < 0 || row < 0 || col > hres || row > vres) {
		return black;
	}

	float r = texture(col, row, 0, 0) / 255.0;
	float g = texture(col, row, 0, 1) / 255.0;
	float b = texture(col, row, 0, 2) / 255.0;

	return Color(r,g,b);
}