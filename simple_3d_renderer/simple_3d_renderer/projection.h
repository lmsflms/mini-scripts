#pragma once
#include "shapes.h"

Point2D perspectiveProjection(const Point3D& vertex, float d);

Cube2D projectCube3D(const Cube3D& cube3D, float d);