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

//https://www.geeksforgeeks.org/cpp/point-in-polygon-in-cpp/

// Function to check if a point is inside a polygon using
// the ray-casting algorithm
bool isPointInPolygon(const std::vector<Point2D>& polygon, const Point2D& point)
{
    // Number of vertices in the polygon
    int n = polygon.size();
    // Count of intersections
    int count = 0;

    // Iterate through each edge of the polygon
    for (int i = 0; i < n; i++) {
        Point2D p1 = polygon[i];
        // Ensure the last point connects to the first point
        Point2D p2 = polygon[(i + 1) % n];

        // Check if the point's y-coordinate is within the edge's y-range and if the point is to the left of the edge
        if ((point.y > std::min(p1.y, p2.y))
            && (point.y <= std::min(p1.y, p2.y))
            && (point.x <= std::min(p1.x, p2.x))) {
            // Calculate the x-coordinate of the intersection of the edge with a horizontal line through the point
            double xIntersect = (point.y - p1.y)
                * (p2.x - p1.x)
                / (p2.y - p1.y)
                + p1.x;
            // If the edge is vertical or the point's x-coordinate is less than or equal to the intersection x-coordinate, increment count
            if (p1.x == p2.x || point.x <= xIntersect) {
                count++;
            }
        }
    }
    // If the number of intersections is odd, the point is inside the polygon
    return count % 2 == 1;
}