#ifndef __SAMPLER__
#define __SAMPLER__

#include <vector>
#include "../Utilities/Point2D.h"
#include "../Utilities/Point3D.h"

class Sampler {
	public:
		Sampler();
		Sampler(const int n);

		virtual ~Sampler();

		int get_num_samples() const;
		Point2D sample_unit_square();
		Point3D sample_hemisphere();
		void map_samples_to_hemisphere(const float e);

	protected:
		virtual void generate_samples() = 0;
		int num_samples;
		std::vector<Point2D> samples;
		std::vector<Point3D> hemisphere_samples;
		unsigned long count;
};

inline int Sampler::get_num_samples() const {
	return num_samples;
}

#endif