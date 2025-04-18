#include "rooflesshouse.h"

rooflessHouse::rooflessHouse(float r, float g, float b) {
    drawType = 0; // 0 = fill, 1 = line

    vertex v1{-1,-1,-1, r,g,b};
    vertex v2{-1,-1,1,  r,g,b};
    vertex v3{-1,1,-1,  r,g,b};
    vertex v4{1,1,1,    r,g,b};
    vertex v5{-1,1,1,   r,g,b};
    vertex v6{1,-1,1,   r,g,b};
    vertex v7{1,1,-1,   r,g,b};
    vertex v8{1,-1,-1,  r,g,b};

    mVertices.push_back(v1); // front
    mVertices.push_back(v3);
    mVertices.push_back(v5);
    mVertices.push_back(v5);
    mVertices.push_back(v2);
    mVertices.push_back(v1);

    mVertices.push_back(v2); // right side
    mVertices.push_back(v5);
    mVertices.push_back(v4);
    mVertices.push_back(v4);
    mVertices.push_back(v6);
    mVertices.push_back(v2);

    mVertices.push_back(v6); // back
    mVertices.push_back(v4);
    mVertices.push_back(v7);
    mVertices.push_back(v7);
    mVertices.push_back(v8);
    mVertices.push_back(v6);

    mVertices.push_back(v7); // left side
    mVertices.push_back(v8);
    mVertices.push_back(v1);
    mVertices.push_back(v1);
    mVertices.push_back(v3);
    mVertices.push_back(v7);
    /*
    mVertices.push_back(v3); // top side
    mVertices.push_back(v7);
    mVertices.push_back(v5);
    mVertices.push_back(v5);
    mVertices.push_back(v4);
    mVertices.push_back(v7);

    mVertices.push_back(v1); // bottom side
    mVertices.push_back(v8);
    mVertices.push_back(v6);
    mVertices.push_back(v6);
    mVertices.push_back(v2);
    mVertices.push_back(v1);
*/
    //Skalerer ned kvadrat i eget kordinatsystem/frame
    //Temporary scale and positioning
    mMatrix.setToIdentity();
    mMatrix.scale(0.5f);
    mMatrix.translate(0.5f, 0.1f, 0.1f);
}
