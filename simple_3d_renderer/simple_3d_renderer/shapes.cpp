#include "shapes.h"

Cube3D::Cube3D(VecPoint3D c_points)
{
	points = c_points;
	originalPoints = c_points;
}

Point3D Cube3D::getCubeCenter()
{
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;

	for (int i = 0; i < (*this).points.size(); i++) {
		sum_x += (*this).points[i].x;
		sum_y += (*this).points[i].y;
		sum_z += (*this).points[i].z;
	}
	return Point3D{ sum_x * 0.125f, sum_y * 0.125f, sum_z * 0.125f };
}
