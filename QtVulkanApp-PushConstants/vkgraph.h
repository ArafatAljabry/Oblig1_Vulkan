#ifndef VKGRAPH_H
#define VKGRAPH_H

#include <string.h>
#include "visualobject.h"
class VKGraph : public VisualObject
{
public:
    VKGraph();
    VKGraph(const std::string& filename);
};

#endif // VKGRAPH_H
