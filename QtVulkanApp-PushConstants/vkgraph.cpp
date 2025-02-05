#include "vkgraph.h"

VKGraph::VKGraph() : VisualObject() {
    vertex v1{0.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f};
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

VKGraph::VKGraph(const std::string& filename) : VisualObject()
{

    std::ifstream inn(filename);
    if (!inn.is_open())
    {
        qDebug("Failed to open file");
        return;
    }


    // read input from math part of compulsory
    int TotalVertices;
    vertex v;

    inn >> TotalVertices; // Gets the first line, which is the total vertices in the file
    for(auto i = 0; i < TotalVertices; i++)
    {

        //Subsequential lines in the file gets added as vertexes.
        inn >> v;
        mVertices.push_back(v);

    }
    qDebug("Added vertex to vector: %f, %f, %f", mVertices[0].x);
    qDebug("Added vertex to vector: %f, %f, %f", mVertices[1].x);

    qDebug("Added vertex to vector: %f, %f, %f", mVertices[2].x);


    inn.close();
}
