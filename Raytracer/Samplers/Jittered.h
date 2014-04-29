#ifndef __JITTERED__
#define __JITTERED__

#include "Sampler.h"

class Jittered : public Sampler {
	public:
		Jittered();
		Jittered(const int n);

		~Jittered();

	private:
		virtual void generate_samples();
};

#endif