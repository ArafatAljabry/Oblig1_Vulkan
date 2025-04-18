#include "vertex.h"


std::ostream& operator<<(std::ostream& os, const vertex& v){
    os << std::fixed;
    os << "(" << v.x << "," << v.y << "," << v.z << v.r << "," << v.g << "," << v.b << ")" << std::endl;


    return os;
}

std::ifstream& operator>>(std::ifstream& is, vertex& v)
{
    //Trenger fire temporære variabler som kun skal lese inn paranteser og komma
    char dum, dum2, dum3, dum4;
    is >> dum >> v.x >> dum2 >> v.y >> dum3 >> v.z >> dum4 >> v.r  >> dum2 >> v.g >> dum3 >> v.b >> dum4;


    return is;
}

vertex::vertex(QVector3D pos, QVector3D color, QVector2D UV)
{
    x = pos.x();
    y = pos.y();
    z = pos.z();

    r = color.x();
    g = color.y();
    b = color.z();

    u = UV.x();
    v = UV.y();
}
