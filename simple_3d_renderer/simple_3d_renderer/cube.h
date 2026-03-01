#pragma once
#include <string>
#include "coords.h"
#include <iostream>

using Vertex = Coord3D;
using string = std::string;

using vector_vertex = std::vector<Coord3D>;
using vector_coord2D = std::vector<Coord2D>;

struct Cube3D {
	vector_vertex vertices = {
		{-1, 1, 1},
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		{ -1, 1, -1 },
		{ -1, -1, 1 },
		{ 1, -1, 1 },
		{ 1, -1, -1 },
		{ -1, -1, -1 }
	};
};

struct Cube2D {
	vector_coord2D points;
};

std::ostream& operator<<(std::ostream os, const Cube2D& c) {
	return os;
}