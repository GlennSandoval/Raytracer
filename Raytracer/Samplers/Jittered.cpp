#include "Jittered.h"
#include "../Utilities/Point2D.h"
#include <vector>

Jittered::Jittered() : Sampler()
{}

Jittered::Jittered(const int n) : Sampler(n)
{
	generate_samples();
	map_samples_to_hemisphere(5);
}

Jittered::~Jittered()
{}

void Jittered::generate_samples() {
	int n = num_samples; //(int)sqrt((float)num_samples);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float r1 = ((float)rand()/(float)RAND_MAX) / n;
			float r2 = ((float)rand()/(float)RAND_MAX) / n;

			samples.push_back(Point2D((i / (float)n) + r1, (j / (float)n) + r2));
		}
	}
}