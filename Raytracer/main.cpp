#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include "World/World.h"
#include "Utilities/CImg.h"

#include <process.h>

void render_scene(World& world) {
	int hres = world.get_camera().hres;
	int vres = world.get_camera().vres;
	
	// determine number of cores
	unsigned concurrentThreadsSupported = std::thread::hardware_concurrency();

	// times
	clock_t start, end;
    float elapsedTime;

    // random seed
    srand((unsigned)time(0));

	// start time
    start = clock();

	float total = (vres / world.get_chunk_size() + 1) * (hres / world.get_chunk_size());
	int percentDone = 0;

	int numChunks = -1;
	std::vector<std::thread> threads;
	while (numChunks < total) {
		for (int i=0; i<concurrentThreadsSupported; i++) {
			if (numChunks < total) threads.push_back(std::thread(&World::render, &world, numChunks++));
			
			// calculate percent done
			int tempPercentDone = ((float)numChunks / total) * 100;
			if (tempPercentDone - percentDone >= 1) {
				percentDone = tempPercentDone;
				std::cout << percentDone << "% complete" << std::endl;
			}
		}

		for (auto& t : threads)
			t.join();
		threads.clear();
	}

	// stop time
    end = clock();
	elapsedTime = (float)(end - start) / CLOCKS_PER_SEC;
	if (elapsedTime > 60.0) {
		int mins = elapsedTime / 60;
		int secs = (int)elapsedTime % 60;
		std::cout << "Completed in: " << mins << " minutes, " << secs << " seconds" << std::endl;
	}
	else {
		std::cout << "Completed in: " << elapsedTime << " s" << std::endl;
	}
}

int main(int argc, char *argv[])
{   
    while (true) {
        std::string filename;
	    std::cout << "Enter scene file: " << std::endl;
	    std::cin >> filename;
    
        World world = World();
        
		if (world.build(filename.c_str())) {
			// ray trace
			render_scene(world);

			cimg_library::CImg<unsigned char> img = *world.get_image();

			cimg_library::CImgDisplay main_disp(img, "Raytracer");

			while (!main_disp.is_closed()) {
				main_disp.wait();
				if (main_disp.key() == cimg_library::cimg::keyS) {
					std::string fn;
					std::cout << "Save as: " << std::endl;
					std::cin >> fn;
					fn = "OutputImages/" + fn;
					img.save(fn.c_str());
					std::cout << "Saved as " << fn << std::endl;
				}
			}
		}
    }
    return 0;
}