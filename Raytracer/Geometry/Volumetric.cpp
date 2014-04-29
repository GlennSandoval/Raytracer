#include "Volumetric.h"

Volumetric::Volumetric() : bbox(BoundingBox()), stepsize(0.01)
{}

Volumetric::Volumetric(const Point3D& point1, const Point3D& point2) : bbox(BoundingBox(point1, point2)), stepsize(0.01)
{}