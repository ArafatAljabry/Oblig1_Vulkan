#include "plane.h"

plane::plane() {

    vertex v1{0,0,0,   0,0,1};
    vertex v2{50,0,0,  0,0,1};
    vertex v6{0,0,50,  0,0,1};
    vertex v7{50,0,50, 0,0,1};


    mVertices.push_back(v1); //bottom
    mVertices.push_back(v2);
    mVertices.push_back(v6);
    mVertices.push_back(v6);
    mVertices.push_back(v7);
    mVertices.push_back(v2);

    //Skalerer ned kvadrat i eget kordinatsystem/frame
    //Temporary scale and positioning
    mMatrix.setToIdentity();
    mMatrix.scale(0.5f);
    mMatrix.translate(0.5f, 0.1f, 0.1f);
}
