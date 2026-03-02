#pragma once
#include <vector>

struct Point2D {
	float x;
	float y;
	Point2D(float x = 0, float y = 0)
		: x(x), y(y){
	};
};

struct Point3D {
	float x;
	float y;
	float z;
	Point3D(float x = 0, float y = 0, float z = 0)
		: x(x), y(y), z(z) {
	};
};