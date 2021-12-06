#include "edge.h"

Edge::Edge(int ivex,int jvex,int w)
{
    this->iVex = ivex;
    this->jVex = jvex;
    this->weight = w;
    this->iLink = nullptr;
    this->jLink = nullptr;
}
