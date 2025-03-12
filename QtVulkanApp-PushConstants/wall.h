#ifndef WALL_H
#define WALL_H
#include "visualobject.h"
class wall : public VisualObject
{
public:
    wall(float r, float g, float b);
    ~wall(){}

public:
    bool isOpen{false};
};

#endif // WALL_H
