#include "../Utilities/Constants.h"
#include "Material.h"


Material::Material()
{}

Color Material::shade(ShadeRec& sr) {
	return black;
}

Color Material::get_Le(const ShadeRec& sr) const {
	return black;
}