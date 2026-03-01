#pragma once
#include <vector>

struct Point2D {
	float x;
	float y;
	Point2D(float cx = 0, float cy = 0)
		: x(cx), y(cy){
	};
};

struct Point3D {
	float x;
	float y;
	float z;
	Point3D(float cx = 0, float cy = 0, float cz = 0)
		: x(cx), y(cy), z(cz) {
	};

	void applyVector(std::vector<float> v);
};