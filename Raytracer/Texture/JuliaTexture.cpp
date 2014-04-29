#include "JuliaTexture.h"
#include "../Utilities/Constants.h"
#include <math.h>

JuliaTexture::JuliaTexture() : size(1024)
{
	values.resize(3 * size * size);
	
	//each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
	float cRe, cIm;                   //real and imaginary part of the constant c, determinate shape of the Julia Set
	float newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
	Color color; //the RGB color value for the pixel
	int maxIterations = 300; //after how much iterations the function should stop

	//pick some values for the constant c, this determines the shape of the Julia Set
	cRe = -0.8;
	cIm = 0.156;

	//loop through every pixel
	for(int x = 0; x < size; x++)
	for(int y = 0; y < size; y++)
	{
		//calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
		newRe = 1.5 * (x - size / 2) / (0.5 * size);
		newIm = (y - size / 2) / (0.5 * size);
		//i will represent the number of iterations
		int i;
		//start the iteration process
		for(i = 0; i < maxIterations; i++)
		{
			//remember value of previous iteration
			oldRe = newRe;
			oldIm = newIm;
			//the actual iteration, the real and imaginary part are calculated
			newRe = oldRe * oldRe - oldIm * oldIm + cRe;
			newIm = 2 * oldRe * oldIm + cIm;
			//if the point is outside the circle with radius 2: stop
			if((newRe * newRe + newIm * newIm) > 4) break;
		}
		//use color model conversion to get rainbow palette, make brightness black if maxIterations reached
		color = Color(i % 256, 255, 255 * (i < maxIterations));

		//draw the pixel
		int index = y * size + x;
		values[index] = color.r / 255.0;
		values[index+1] = color.g / 255.0;
		values[index+2] = color.b / 255.0;
	}
}

Color JuliaTexture::get_color(const ShadeRec& sr) const {
	int row, col;

	if (mapping_ptr)
		mapping_ptr->get_pixel_coordinates(sr, size, size, row, col);
	else {
		row = (int) (sr.v * (size - 1));
		col = (int) (sr.u * (size - 1));
	}

	if (col < 0 || row < 0 || col > size || row > size) {
		return black;
	}

	int index = row * size + col;
	float r = values[index];
	float g = values[index+1];
	float b = values[index+2];

	return Color(r,g,b);
}