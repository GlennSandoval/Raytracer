#include "Camera.h"

Camera::Camera() : pe(Point3D(-1,2,4)), vUp(Vector3D(0,1,0)), d(1),
					hres(640), vres(480), sx(1), sy(1.5), f(5), lens_radius(0.05)
{
	vView = (Point3D(0,0,0) - pe).normalized();
	compute_coords();
}

Camera::Camera(const Point3D p, float d, int hres, int vres, int sx, int sy, float f, float lr) : pe(p), vUp(Vector3D(0,1,0)), d(d),
					hres(hres), vres(vres), sx(sx), sy(sy), f(f), lens_radius(lr)
{
	compute_coords();
}

void Camera::compute_coords() {
	vView = (Point3D(0,0,0) - pe).normalized();
	
	w = vView.normalized();
	u = w ^ vUp;
	u.normalize();
	v = u ^ w;
	v = -v;

	// for stereo
	//pe = pe - 0.15 * u;

	pc = pe + d * w;
	p0 = pc - (0.5 * sx * u) - (0.5 * sy * v);
}