#include "box.h"

box::box() {

    drawType = 0; // 0 = fill, 1 = line

    vertex v1{-1,-1,-1, 0,1,0};
    vertex v2{-1,-1,1, 0,1,0};
    vertex v3{-1,1,-1, 0,1,0};
    vertex v4{1,1,1, 0,1,0};
    vertex v5{-1,1,1, 0,1,0};
    vertex v6{1,-1,1, 0,1,0};
    vertex v7{1,1,-1, 0,1,0};
    vertex v8{1,-1,-1, 0,1,0};

    mVertices.push_back(v1); //left side
    mVertices.push_back(v3);
    mVertices.push_back(v5);
    mVertices.push_back(v5);
    mVertices.push_back(v2);
    mVertices.push_back(v1);

    mVertices.push_back(v2);//back
    mVertices.push_back(v5);
    mVertices.push_back(v4);
    mVertices.push_back(v4);
    mVertices.push_back(v6);
    mVertices.push_back(v2);

    mVertices.push_back(v6);//front
    mVertices.push_back(v4);
    mVertices.push_back(v7);
    mVertices.push_back(v7);
    mVertices.push_back(v8);
    mVertices.push_back(v6);

    mVertices.push_back(v7);//right side
    mVertices.push_back(v8);
    mVertices.push_back(v1);
    mVertices.push_back(v1);
    mVertices.push_back(v3);
    mVertices.push_back(v7);

    mVertices.push_back(v3); //bottom
    mVertices.push_back(v7);
    mVertices.push_back(v5);
    mVertices.push_back(v5);
    mVertices.push_back(v4);
    mVertices.push_back(v7);

    mVertices.push_back(v1); //top
    mVertices.push_back(v8);
    mVertices.push_back(v6);
    mVertices.push_back(v6);
    mVertices.push_back(v3);
    mVertices.push_back(v1);

    //Skalerer ned kvadrat i eget kordinatsystem/frame
    //Temporary scale and positioning
    mMatrix.setToIdentity();
    mMatrix.scale(0.5f);
    mMatrix.translate(0.5f, 0.1f, 0.1f);
}
