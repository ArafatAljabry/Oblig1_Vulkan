#include "visualobject.h"

VisualObject::VisualObject() {
     mMatrix.setToIdentity();
}

void VisualObject::rotate(float t, float x, float y, float z)
{
    mMatrix.rotate(t,x,y,z);
}

/**
 * @brief Move the object around
 * @param Movement in x axis
 * @param Movement in y, default to 0
 * @param Movement in z, default to 0
 */
void VisualObject::move(float x, float y, float z)
{
   /* mMatrix.translate(
                        getPosition().x() + x,
                        getPosition().y() + y,
                        getPosition().z() + z
                    );
*/
    mMatrix.translate(x,y,z);
}

QVector3D VisualObject::getPosition()
{
    return QVector3D(mMatrix(0,3), mMatrix(1,3), mMatrix(2,3));
}
/**
 * @brief Sclae the object with a value
 * @param float to scale with
 */
void VisualObject::scale(float s)
{
    mMatrix.scale(s);
}

void VisualObject::setTag(std::string tag)
{
    mTag = tag;
}

