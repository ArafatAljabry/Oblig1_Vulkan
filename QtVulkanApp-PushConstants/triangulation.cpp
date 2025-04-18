#include "triangulation.h"


triangulation::triangulation() {
    drawType = 0; // 0 = fill, 1 = line

    // Compute Delaunay Triangulation
    result = delaunayTriangulation(points);

    // Output the triangles
    std::cout << "Delaunay Triangles:\n";
    for (const auto& tri : result) {
        /*
        std::cout << "Triangle: (" << tri.a.x << ", " << tri.a.y << ") - "
                  << "(" << tri.b.x << ", " << tri.b.y << ") - "
                  << "(" << tri.c.x << ", " << tri.c.y << ")\n";
        )*/


        vertex v1 {static_cast<float>(tri.a.x),static_cast<float>(tri.a.y),0,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX};
        vertex v2 {static_cast<float>(tri.b.x),static_cast<float>(tri.b.y),0,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX};
        vertex v3 {static_cast<float>(tri.c.x),static_cast<float>(tri.c.y),0,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX};

        mVertices.push_back(v1);
        mVertices.push_back(v2);
        mVertices.push_back(v3);


    }
}


// Function to check if a point is inside the circumcircle of a given triangle
bool triangulation::isPointInCircumcircle(const Triangle& tri, const Point& p) {
    double ax = tri.a.x - p.x, ay = tri.a.y - p.y;
    double bx = tri.b.x - p.x, by = tri.b.y - p.y;
    double cx = tri.c.x - p.x, cy = tri.c.y - p.y;
    double det = (ax * (by * (cx * cx + cy * cy) - cy * (bx * bx + by * by)) -
                  ay * (bx * (cx * cx + cy * cy) - cx * (bx * bx + by * by)) +
                  (ax * ax + ay * ay) * (bx * cy - by * cx));
    return det > 0;
}
// Generate a super-triangle that covers all points
Triangle triangulation::getSuperTriangle(const std::vector<Point>& points) {
    double minX = points[0].x, minY = points[0].y;
    double maxX = minX, maxY = minY;
    for (const auto& p : points) {
        minX = std::min(minX, p.x);
        minY = std::min(minY, p.y);
        maxX = std::max(maxX, p.x);
        maxY = std::max(maxY, p.y);
    }
    double dx = maxX - minX, dy = maxY - minY;
    double dmax = std::max(dx, dy) * 2.0;
    return { {minX - dmax, minY - dmax},
            {minX + dmax * 2, minY - dmax},
            {minX + dmax, minY + dmax * 2} };
}
// Perform Delaunay Triangulation using Bowyer-Watson algorithm
std::vector<Triangle> triangulation::delaunayTriangulation(std::vector<Point>& points) {
    std::vector<Triangle> triangles;
    Triangle superTriangle = getSuperTriangle(points);
    triangles.push_back(superTriangle);
    for (const auto& p : points) {
        std::vector<Triangle> badTriangles;
        std::vector<std::pair<Point, Point>> edges;
        for (const auto& tri : triangles) {
            if (isPointInCircumcircle(tri, p)) {
                badTriangles.push_back(tri);
                edges.push_back({ tri.a, tri.b });
                edges.push_back({ tri.b, tri.c });
                edges.push_back({ tri.c, tri.a });
            }
        }
        // Remove bad triangles
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
                                       [&badTriangles](const Triangle& t) {
                                           return std::find(badTriangles.begin(), badTriangles.end(), t) !=
                                                  badTriangles.end();
                                       }), triangles.end());
        // Reconstruct triangles
        for (const auto& edge : edges) {
            triangles.push_back({ edge.first, edge.second, p });
        }
    }
    // Remove super-triangle-related triangles
    triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
                                   [&superTriangle](const Triangle& t) {
                                       return (t.a == superTriangle.a || t.b == superTriangle.a || t.c ==
                                                                                                       superTriangle.a ||
                                               t.a == superTriangle.b || t.b == superTriangle.b || t.c ==
                                                                                                       superTriangle.b ||
                                               t.a == superTriangle.c || t.b == superTriangle.c || t.c ==
                                                                                                       superTriangle.c);
                                   }), triangles.end());
    return triangles;
}
