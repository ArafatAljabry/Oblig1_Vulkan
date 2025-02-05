#include "vktriangle.h"

VkTriangle::VkTriangle() :VisualObject() {

    //Y up, front = CCW
    // X, Y, Z  R, G, B   U, V
    vertex v1{-1.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f};
    vertex v2{1.0f,   0.0f,  0.0f,   0.0f, 1.0f, 0.0f};
    vertex v3{0.0f,   1.0f,  0.0f,   0.0f, 0.0f, 1.0f};
    vertex v4{1.0f,   1.0f,  0.0f,   1.0f, 1.0f, 0.0f};

    mVertices.push_back(v1);
    mVertices.push_back(v2);
    mVertices.push_back(v3);
    mVertices.push_back(v3);
    mVertices.push_back(v2);
    mVertices.push_back(v4);
}
