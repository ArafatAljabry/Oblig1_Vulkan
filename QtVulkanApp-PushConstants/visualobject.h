#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QVulkanWindow> // VkDeviceMemory, VkBuffer defined here
#include <vector>
#include "vertex.h"

class VisualObject
{
public:
    VisualObject();

    std::vector<vertex> mVertices;
    std::vector<vertex> getVertices(){return mVertices;}


    VkDeviceMemory mBufferMemory{VK_NULL_HANDLE};
    VkBuffer mBuffer{VK_NULL_HANDLE};
    VkPrimitiveTopology mTopology{VK_PRIMITIVE_TOPOLOGY_LINE_LIST};

    QMatrix4x4 mMatrix;

    //Getter and setter for name
    void setName(std::string name){mName = name;}
    std::string getName() const{return mName;}

    //Functions for moving, scaling and rotating the object

    void move(float x, float y = 0.0f, float z = 0.0f);
    void scale(float s);
    void rotate(float t, float x, float y, float z);

    int drawType{0}; // 0 = fill, 1 = line

    QVector3D mColor{0.f,0.f,0.f};

protected:
    std::string mName;

};

#endif // VISUALOBJECT_H
