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

    //for collision
    float radius{0.5f};

    VkDeviceMemory mBufferMemory{VK_NULL_HANDLE};
    VkBuffer mBuffer{VK_NULL_HANDLE};
    VkPrimitiveTopology mTopology{VK_PRIMITIVE_TOPOLOGY_LINE_LIST};

    QMatrix4x4 mMatrix;
    QVector3D getPosition();
    //Getter and setter for name
    void setName(std::string name){mName = name;}
    std::string getName() const{return mName;}


    //Functions for moving, scaling and rotating the object

    void move(float x, float y = 0.0f, float z = 0.0f);
    void scale(float s);
    void rotate(float t, float x, float y, float z);

    std::string getTag() const{return mTag;}
    void setTag(std::string tag);

    bool enableCollision{true}; //Won't trigger collision logic if false
    int drawType{0}; // 0 = fill, 1 = line

    QVector3D mColor{0.f,0.f,0.f};
    inline std::vector<vertex> getVertices() const { return mVertices; }
    inline std::vector<uint32_t> getIndices() const { return mIndices; }


    inline VkBuffer& getIBuffer() { return mIndexBuffer.mBuffer; }
    inline VkDeviceMemory& getIBufferMemory() { return mIndexBuffer.mBufferMemory; }
    inline void setIBuffer(VkBuffer bufferIn) { mIndexBuffer.mBuffer = bufferIn; }
    inline void setIBufferMemory(VkDeviceMemory bufferMemoryIn) { mIndexBuffer.mBufferMemory = bufferMemoryIn; }

    //for the door,would be nice on the wall class, to be continued
    bool isOpen{false};

protected:
    std::string mName;
    std::string mTag{"actor"};
    std::vector<uint32_t> mIndices;

};

#endif // VISUALOBJECT_H
