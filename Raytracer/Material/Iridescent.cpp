#include "Iridescent.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector3D.h"
#include "../World/World.h"

Iridescent::Iridescent() : Phong()
{
	WL_HIGH = 720;
	WL_LOW = 380;
	WL_SKIP = 10;
	NM = 1e-9f;
	n = 1.5;
	d = 200 * NM;

	for (float i = WL_LOW; i < WL_HIGH; i += WL_SKIP) {
		colors.push_back(WLToColor(i));
		wls.push_back(i);
	}
}

Color Iridescent::shade(ShadeRec& sr) {
	Color col = black;
	Color color = black;
	float s = 0.0;
	float c = 0.0;

	Vector3D v = -sr.ray.d;
	Vector3D r = sr.ray.d + 2.0f * (sr.nh * v) * sr.nh;

	///
	Vector3D n2 = sr.nh;

	float cosTheta1 = sr.nh * v;
	float theeta = eta;
	if(cosTheta1 < 0)
	{
		n2 = -n2;
		cosTheta1 = -cosTheta1;
		theeta = 1.0f / theeta;
	}

	float a = -1.0f / theeta;
	float term = ((cosTheta1 * cosTheta1 -1) / (theeta * theeta)) + 1;
	
	Vector3D transmitDir;
	if (term > 0.0) {
		float b = (cosTheta1 / theeta) - sqrt(term);
		transmitDir = a * v + b * n2;
		transmitDir.normalize();
	}
	else {
		transmitDir = -v + 2.0f * cosTheta1 * n2;
		transmitDir.normalize();
	}

	Ray transmittedRay = Ray(sr.ph+0.01f*transmitDir, transmitDir);
	///


	float cosTheta = r * sr.nh;
	float opd = 2 * n * d * cosTheta;

	Color total = black;
	for (unsigned int i=0; i < colors.size(); i++) {
		float m = opd / (wls[i] * NM);
		m = (float)((int)(m * 2) % 2);
		if (m < 0) m += 2;
		if (m > 1) m = 2 - m;
		m = 2 * m;
		total += m * colors[i];
	}

	col += total * (1.0f / colors.size()) * sr.world.trace_ray(transmittedRay, sr.depth + 1);

	return col;

	
}

Color Iridescent::WLToColor(float wl) {
	if (wl >= 380 && wl <= 440)
		return Color((440-wl) / (440-380), 0, 1);
	else if (wl >= 440 && wl <= 490)
		return Color(0, (wl-440)/(490-440), 1);
	else if (wl >= 490 && wl <= 510)
		return Color(0, 1, (510-wl) / (510-490));
	else if (wl >= 510 && wl <= 580)
		return Color((wl-510) / (580-510), 1, 0);
	else if (wl >= 580 && wl <= 645)
		return Color(1, (645-wl)/(645-580), 0);
	else if (wl >= 645 && wl <= 780)
		return Color(1,0,0);
	return black;
}