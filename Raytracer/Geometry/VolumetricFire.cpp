#include "VolumetricFire.h"
#include "../Utilities/Constants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

VolumetricFire::VolumetricFire() : fire(Fire(1))
{}

VolumetricFire::VolumetricFire(const char* filename) : scale(2.0), fire(Fire(1)) {
	bbox = BoundingBox();
	smokeColor = Color(0.2, 0.2, 0.2);
	std::ifstream file;
	file.open(filename);

	int dim;
	file >> dim;
	dim_x = dim;
	file >> dim;
	dim_y = dim;
	file >> dim;
	dim_z = dim;

	float dens;

	for (int i=0; i < dim_x * dim_y * dim_z; i++) {
		file >> dens;
		data.push_back(dens);
	}

	file.close();
	std::cout << "VolumetricFire data read" << std::endl;

	file.open("smoke.txt");

	file >> dim;
	dim_x = dim;
	file >> dim;
	dim_y = dim;
	file >> dim;
	dim_z = dim;

	for (int i=0; i < dim_x * dim_y * dim_z; i++) {
		file >> dens;
		smoke.push_back(dens);
	}

	file.close();
	std::cout << "Smoke data read" << std::endl;

	stepsize = scale / dim_x;
}

VolumetricFire::VolumetricFire(Fire& f) : scale(2.0), fire(f) {
	bbox = BoundingBox();
	smokeColor = Color(0.2, 0.2, 0.2);
	
	dim_x = fire.dim, dim_y = fire.dim, dim_z = fire.dim;
	data = fire.t;
	smoke = fire.d;
	fuel = fire.fuel;

	stepsize = scale / dim_x;
}

/* from http://www.fourmilab.ch/documents/specrend/ 
	* via http://www.scratchapixel.com/lessons/3d-advanced-lessons/blackbody/spectrum-of-blackbodies/
	*/
static double colorMatchingFunc[][3]={
	{0.0014,0.0000,0.0065}, {0.0022,0.0001,0.0105}, {0.0042,0.0001,0.0201},
	{0.0076,0.0002,0.0362}, {0.0143,0.0004,0.0679}, {0.0232,0.0006,0.1102},
	{0.0435,0.0012,0.2074}, {0.0776,0.0022,0.3713}, {0.1344,0.0040,0.6456},
	{0.2148,0.0073,1.0391}, {0.2839,0.0116,1.3856}, {0.3285,0.0168,1.6230},
	{0.3483,0.0230,1.7471}, {0.3481,0.0298,1.7826}, {0.3362,0.0380,1.7721},
	{0.3187,0.0480,1.7441}, {0.2908,0.0600,1.6692}, {0.2511,0.0739,1.5281},
	{0.1954,0.0910,1.2876}, {0.1421,0.1126,1.0419}, {0.0956,0.1390,0.8130},
	{0.0580,0.1693,0.6162}, {0.0320,0.2080,0.4652}, {0.0147,0.2586,0.3533},
	{0.0049,0.3230,0.2720}, {0.0024,0.4073,0.2123}, {0.0093,0.5030,0.1582},
	{0.0291,0.6082,0.1117}, {0.0633,0.7100,0.0782}, {0.1096,0.7932,0.0573},
	{0.1655,0.8620,0.0422}, {0.2257,0.9149,0.0298}, {0.2904,0.9540,0.0203},
	{0.3597,0.9803,0.0134}, {0.4334,0.9950,0.0087}, {0.5121,1.0000,0.0057},
	{0.5945,0.9950,0.0039}, {0.6784,0.9786,0.0027}, {0.7621,0.9520,0.0021},
	{0.8425,0.9154,0.0018}, {0.9163,0.8700,0.0017}, {0.9786,0.8163,0.0014},
	{1.0263,0.7570,0.0011}, {1.0567,0.6949,0.0010}, {1.0622,0.6310,0.0008},
	{1.0456,0.5668,0.0006}, {1.0026,0.5030,0.0003}, {0.9384,0.4412,0.0002},
	{0.8544,0.3810,0.0002}, {0.7514,0.3210,0.0001}, {0.6424,0.2650,0.0000},
	{0.5419,0.2170,0.0000}, {0.4479,0.1750,0.0000}, {0.3608,0.1382,0.0000},
	{0.2835,0.1070,0.0000}, {0.2187,0.0816,0.0000}, {0.1649,0.0610,0.0000},
	{0.1212,0.0446,0.0000}, {0.0874,0.0320,0.0000}, {0.0636,0.0232,0.0000},
	{0.0468,0.0170,0.0000}, {0.0329,0.0119,0.0000}, {0.0227,0.0082,0.0000},
	{0.0158,0.0057,0.0000}, {0.0114,0.0041,0.0000}, {0.0081,0.0029,0.0000},
	{0.0058,0.0021,0.0000}, {0.0041,0.0015,0.0000}, {0.0029,0.0010,0.0000},
	{0.0020,0.0007,0.0000}, {0.0014,0.0005,0.0000}, {0.0010,0.0004,0.0000},
	{0.0007,0.0002,0.0000}, {0.0005,0.0002,0.0000}, {0.0003,0.0001,0.0000},
	{0.0002,0.0001,0.0000}, {0.0002,0.0001,0.0000}, {0.0001,0.0000,0.0000},
	{0.0001,0.0000,0.0000}, {0.0001,0.0000,0.0000}, {0.0000,0.0000,0.0000}
};

float VolumetricFire::evaluate(const Point3D& point) const {
	float factor = (dim_x / 2.0);
	Point3D p = Point3D(factor * (point.x + 1.0), factor * (point.y + 1.0), factor * (point.z + 1.0)); 
	int x = (int)p.x;
	int y = (int)p.y;
	int z = (int)p.z;

	float dx = p.x - x;
	float dy = p.y - y;
	float dz = p.z - z;

	float interpolated = (1-dx) * (1-dy) * (1-dz) * fire.t[I(  x,  y,  z)] +
							(1-dx) *    dy  * (1-dz) * fire.t[I(  x,y+1,  z)] +
						 	dx  * (1-dy) * (1-dz) * fire.t[I(x+1,  y,  z)] +
						 	dx  *    dy  * (1-dz) * fire.t[I(x+1,y+1,  z)] +
							(1-dx) * (1-dy) *    dz  * fire.t[I(  x,  y,z+1)] +
							(1-dx) *    dy  *    dz  * fire.t[I(  x,y+1,z+1)] +
						 	dx  * (1-dy) *    dz  * fire.t[I(x+1,  y,z+1)] +
						 	dx  *    dy  *    dz  * fire.t[I(x+1,y+1,z+1)];


	return interpolated;
}

float VolumetricFire::blackbody_spectrum(float wavelength, float temp) const {
	float h = 6.62606896e-34;   // Plank constant
	float c = 2.99792458e+8;    // Speed of light
	float k = 1.38064880e-23;   // Boltzmann constant
	float arg1 = 2 * PI * h * c * c;
	float arg2 = ( h * c ) / k;
	return (arg1 * pow((double)wavelength, -5.0)) / (exp(arg2 / (wavelength * temp)) - 1.0);
}

Color VolumetricFire::XYZtoRGB(const float &X, const float &Y, const float &Z) const {
	return Color(X *  3.2406 + Y * -1.5372 + Z * -0.4986,
							X * -0.9689 + Y *  1.8758 + Z *  0.0415,
							X *  0.0557 + Y * -0.2040 + Z *  1.0570 );
}

Color VolumetricFire::calculate_blackbody(const float &temp) const {
	int power = 0;
	float X = 0, Y = 0, Z = 0;
	float minWavelength = 380;
	float maxWavelength = 780;
	int step = 5;
	int steps = (maxWavelength - minWavelength) / step + 1;
	for (int i = 0; i < steps; i++) {
		float lambda = (minWavelength + i * step) * 1e-9;
		float I = blackbody_spectrum(lambda, temp);
		if (I > 0) {
			int d = 0;
		}
		power += I * step;
		X += I * colorMatchingFunc[i][0];
		Y += I * colorMatchingFunc[i][1];
		Z += I * colorMatchingFunc[i][2];
	}
	power /= (float)steps;

	float mag = 1.0 / (max(X, max(Y,Z)));

	X *= mag;
	Y *= mag;
	Z *= mag;
	
	Color result = XYZtoRGB(X, Y, Z);
	result.clamp();

	return result;
}

bool VolumetricFire::hit(const Ray& ray, float& t, float& tmin, ShadeRec& sr) const {
	// TODO: this will only calculate first intersection
	if (bbox.hit(ray)) {
		Normal normal = Normal();
		Point3D hit_pt = ray.o; // + t * ray.d;

		float temp = 0.0, st=0.1, dens=0.0, fuelAmt = 0.0;
		Color color = black;
		bool inside = false, hit = false, smokeHit = false;
		for (int i=0; i < 1000; i++) {
			if (bbox.inside(hit_pt)) {
				inside = true;
				temp = evaluate(hit_pt);
				dens = getSmoke(hit_pt);
				if (temp > 1000) {
					hit = true;
					fuelAmt = 0.5 * getFuel(hit_pt);
					color += fuelAmt * calculate_blackbody(temp);
				}
				if (dens > 0.005) {
					smokeHit = true;
					color += dens * smokeColor;
				}
			}
			hit_pt = ray.o + st * ray.d;
			st += stepsize;
			if (inside && !bbox.inside(hit_pt)) break;
		}
		if (hit || smokeHit) {
			sr.accumulate = true;
			sr.ph = hit_pt;
			sr.local_ph = hit_pt;
			sr.nh = normal;
			sr.color = color;
			tmin = st;
			return true;
		}
		return false;
	}
	return false;
}

bool VolumetricFire::shadow_hit(const Ray& ray, float& tmin, float& Li) const {
	return false;
}

int VolumetricFire::I(int x, int y, int z) const {
	int index = z * dim_z * dim_y + y * dim_x + x;
	if (index < 0) return 0;
	if (index > data.size()) return data.size() - 1;
	return index;
}

float VolumetricFire::getSmoke(const Point3D& point) const {
	float factor = (dim_x / 2.0);
	Point3D p = Point3D(factor * (point.x + 1.0), factor * (point.y + 1.0), factor * (point.z + 1.0)); 
	int x = (int)p.x;
	int y = (int)p.y;
	int z = (int)p.z;

	float dx = p.x - x;
	float dy = p.y - y;
	float dz = p.z - z;

	float interpolated = (1-dx) * (1-dy) * (1-dz) * fire.d[I(  x,  y,  z)] +
							(1-dx) *    dy  * (1-dz) * fire.d[I(  x,y+1,  z)] +
						 	dx  * (1-dy) * (1-dz) * fire.d[I(x+1,  y,  z)] +
						 	dx  *    dy  * (1-dz) * fire.d[I(x+1,y+1,  z)] +
							(1-dx) * (1-dy) *    dz  * fire.d[I(  x,  y,z+1)] +
							(1-dx) *    dy  *    dz  * fire.d[I(  x,y+1,z+1)] +
						 	dx  * (1-dy) *    dz  * fire.d[I(x+1,  y,z+1)] +
						 	dx  *    dy  *    dz  * fire.d[I(x+1,y+1,z+1)];


	return interpolated;
}

float VolumetricFire::getFuel(const Point3D& point) const {
	float factor = (dim_x / 2.0);
	Point3D p = Point3D(factor * (point.x + 1.0), factor * (point.y + 1.0), factor * (point.z + 1.0)); 
	int x = (int)p.x;
	int y = (int)p.y;
	int z = (int)p.z;

	float dx = p.x - x;
	float dy = p.y - y;
	float dz = p.z - z;

	float interpolated = (1-dx) * (1-dy) * (1-dz) * fire.fuel[I(  x,  y,  z)] +
							(1-dx) *    dy  * (1-dz) * fire.fuel[I(  x,y+1,  z)] +
						 	dx  * (1-dy) * (1-dz) * fire.fuel[I(x+1,  y,  z)] +
						 	dx  *    dy  * (1-dz) * fire.fuel[I(x+1,y+1,  z)] +
							(1-dx) * (1-dy) *    dz  * fire.fuel[I(  x,  y,z+1)] +
							(1-dx) *    dy  *    dz  * fire.fuel[I(  x,y+1,z+1)] +
						 	dx  * (1-dy) *    dz  * fire.fuel[I(x+1,  y,z+1)] +
						 	dx  *    dy  *    dz  * fire.fuel[I(x+1,y+1,z+1)];


	return interpolated;
}