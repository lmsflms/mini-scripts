#include "objects.h"

void Point3D::applyVector(std::vector<float> v)
{
	x += v[0];
	y += v[1];
	z += v[2];
}