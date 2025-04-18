#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "visualobject.h"



class triangulation : public VisualObject
{
public:
    triangulation();

    // Compute Delaunay Triangulation
    std::vector<Triangle> result;
    //Sample input
    std::vector<Point> points= {{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}, {0.2, 0.3}, {0.7, 0.8}};

    bool isPointInCircumcircle(const Triangle& tri, const Point& P);
    Triangle getSuperTriangle(const std::vector<Point>& points);
    std::vector<Triangle> delaunayTriangulation(std::vector<Point>& points);
};

#endif // TRIANGULATION_H
