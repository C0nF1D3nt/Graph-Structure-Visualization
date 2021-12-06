#include "vertex.h"

Vertex::Vertex(QString str,int idx)
{
    this->data = str;
    this->id = idx;
    this->firstedge = nullptr;
    this->flag = 0;
}
