#pragma once
#include <vector>

struct Coord2D {
	float x;
	float y;
	Coord2D(float cx = 0, float cy = 0)
		: x(cx), y(cy){
	};
};

struct Coord3D {
	float x;
	float y;
	float z;
	Coord3D(float cx = 0, float cy = 0, float cz = 0)
		: x(cx), y(cy), z(cz) {
	};

	void applyVector(std::vector<float> v);
};