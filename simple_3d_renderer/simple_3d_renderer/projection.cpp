#include "projection.h"

Point2D perspectiveProjection(const Point3D& vertex, float d)
{
    return Point2D(vertex.x * (d / vertex.z), vertex.y * (d / vertex.z));
}

//Projects each vertex of the 3D into a new 2D cube object.
Cube2D projectCube3D(const Cube3D& cube3D, float d)
{
    Cube2D cube2D;

    //for each vertices in cube3D's points vector, we apply perspectiveProjection.
    for (auto v : cube3D.points) {
        cube2D.points.push_back(perspectiveProjection(v, d));
    }

    return cube2D;
}
