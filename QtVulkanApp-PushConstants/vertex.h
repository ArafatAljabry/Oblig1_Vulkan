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

#endif // VERTEX_H
