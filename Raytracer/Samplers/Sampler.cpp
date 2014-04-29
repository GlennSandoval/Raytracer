#include "Sampler.h"
#include "../Utilities/Constants.h"

Sampler::Sampler() : num_samples(1), count(0)
{
	samples = std::vector<Point2D>();
	hemisphere_samples = std::vector<Point3D>();
}

Sampler::Sampler(int n) : num_samples(n), count(0)
{}

Sampler::~Sampler()
{}

Point2D Sampler::sample_unit_square() {
	return samples[count++ % num_samples];
}

Point3D Sampler::sample_hemisphere() {
	return hemisphere_samples[count++ % num_samples];
}

void Sampler::map_samples_to_hemisphere(const float e) {
	/*vector<Point3D*> geodesic_pts = geodesic_ptr->get_points();

	for (int i=0; i < geodesic_pts.size(); i++) {
		hemisphere_samples.push_back(*geodesic_pts[i]);
	}*/

	int size = samples.size();

	for (int i=0; i < size; i++) {
		float cos_phi = cos(2.0 * PI * samples[i].x);
		float sin_phi = sin(2.0 * PI * samples[i].x);
		float cos_theta = pow(1.0 - samples[i].y, 1.0 / (e + 1.0));
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;

		hemisphere_samples.push_back(Point3D(pu, pv, pw));
	}
}