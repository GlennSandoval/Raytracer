#ifndef __CAMERA__
#define __CAMERA__

#include "../Utilities/Point3D.h"
#include "../Utilities/Vector3D.h"
#include "../Utilities/Point2D.h"

class Camera {
	public:
		Point3D pe;
		Vector3D vView;
		Vector3D vUp;

		int hres;
		int vres;
		float sx;
		float sy;

		Vector3D u, v, w; // coordinate system

		Point3D pc;
		Point3D p0;

		int d;
		float f;

		float lens_radius;
		
		Camera();
		Camera(const Point3D p, float d, int hres, int vres, int sx, int sy, float f, float lr);
		void compute_coords();
};
#endif