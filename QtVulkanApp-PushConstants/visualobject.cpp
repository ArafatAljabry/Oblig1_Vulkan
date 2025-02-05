#include "visualobject.h"

VisualObject::VisualObject() {}

void VisualObject::rotate(float t, float x, float y, float z)
{
    mMatrix.rotate(t,x,y,z);
}

