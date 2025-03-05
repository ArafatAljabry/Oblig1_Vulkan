#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QVulkanWindow> // VkDeviceMemory, VkBuffer defined here
#include <vector>
#include "vertex.h"

class VisualObject
{
public:
    std::vector<vertex> mVertices;
    std::vector<vertex> getVertices(){return mVertices;}
    VisualObject();

    VkDeviceMemory mBufferMemory{VK_NULL_HANDLE};
    VkBuffer mBuffer{VK_NULL_HANDLE};
    VkPrimitiveTopology mTopology{VK_PRIMITIVE_TOPOLOGY_LINE_LIST};

    QMatrix4x4 mMatrix;

    //Functions for moving, scaling and rotating the object

    void move(float x, float y = 0.0f, float z = 0.0f);
    void scale(float s);
    void rotate(float t, float x, float y, float z);

};

#endif // VISUALOBJECT_H
