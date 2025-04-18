#ifndef MESHOBJ_H
#define MESHOBJ_H

#include <string>
#include "visualobject.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <QDebug>
#include <QVector3D>
class meshObj : public VisualObject
{
public:
    meshObj(const std::string& filename);

private:
    bool readObjFile(const std::string& filename);
};

#endif // MESHOBJ_H
