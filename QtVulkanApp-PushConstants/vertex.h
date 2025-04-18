#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>
#include <iostream>
#include <fstream>
#

struct vertex{
    float x,y,z, //Position
          r,g,b,// Color
          u = 0 ,v = 0; // Texture coordinates


    vertex() = default;
    vertex(QVector3D pos, QVector3D normal, QVector2D uv);
    vertex(float x, float y, float z, float r, float g, float b, float u, float v)
        : x(x), y(y), z(z), r(r), g(g), b(b), u(u), v(v) {}

    //! Overload ostream operator which writes vertex data on a open textfile stream
    friend std::ostream& operator<<(std::ostream&, const vertex& v);

    //! Overload ostream operator which read all vertex data on a open textfile stream
    friend std::ifstream& operator>>(std::ifstream&, vertex& v);
};

#endif // VERTEX_H
