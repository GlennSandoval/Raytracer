#ifndef __REFRACTIVE__
#define __REFRACTIVE__

#include "Phong.h"

class Refractive : public Phong {
	public:
		Refractive();
		virtual Color shade(ShadeRec& sr);
		float eta;
		float transmission;
		void set_refraction(Texture* tp);
	
	private:
		Texture* refraction_ptr;
};

inline void Refractive::set_refraction(Texture* tp) {
	refraction_ptr = tp;
}

#endif