#ifndef VERTEX_H
#define VERTEX_H


#include <iostream>
#include <fstream>

struct vertex{
    float x,y,z,r,g,b;

    //! Overload ostream operator which writes vertex data on a open textfile stream
    friend std::ostream& operator<<(std::ostream&, const vertex& v);

    //! Overload ostream operator which read all vertex data on a open textfile stream
    friend std::ifstream& operator>>(std::ifstream&, vertex& v);
};

// Structure to represent a 2D point
struct Point {
    double x, y;
    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6;
    }
};
// Structure to represent a triangle
struct Triangle {
    Point a, b, c;
    bool operator==(const Triangle& other) const {
        return (a == other.a && b == other.b && c == other.c) ||
               (a == other.a && b == other.c && c == other.b) ||
               (a == other.b && b == other.a && c == other.c) ||
               (a == other.b && b == other.c && c == other.a) ||
               (a == other.c && b == other.a && c == other.b) ||
               (a == other.c && b == other.b && c == other.a);
    }
};
#endif // VERTEX_H
