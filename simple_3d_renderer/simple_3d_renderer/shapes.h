#pragma once
#include <string>
#include "objects.h"

using Vertex = Point3D;
using string = std::string;

using VecPoint3D = std::vector<Point3D>;
using VecPoint2D = std::vector<Point2D>;
using VecFaces = std::vector<std::vector<int>>; //ids

struct Cube3D {
	VecPoint3D points;

	//Considering the points are listed in the following order:
	//	1. Top front left
	//	2. Top front right
	//	3. Top back right
	//	4. Top back left
	//	5. Bottom front left
	//	6. Bottom front right
	//	7. Bottom back right
	//	8. Bottom back left

	VecFaces faces = {
		{0,1,2,3}, //top
		{4,5,6,7}, //bottom
		{0,1,5,4}, //front
		{1,2,6,5}, //right
		{2,3,7,6}, //back
		{3,0,4,7} //left
		};

	Cube3D(VecPoint3D c_points = {
		{-1, 1, 1},
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		{ -1, 1, -1 },
		{ -1, -1, 1 },
		{ 1, -1, 1 },
		{ 1, -1, -1 },
		{ -1, -1, -1 }
		}
	);
};

struct Cube2D {
	VecPoint2D points;
	VecFaces faces = {
		{0,1,2,3}, //top
		{4,5,6,7}, //bottom
		{0,1,5,4}, //front
		{1,2,6,5}, //right
		{2,3,7,6}, //back
		{3,0,4,7} //left
	};
};