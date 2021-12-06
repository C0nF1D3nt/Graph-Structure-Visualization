#include "alvertex.h"

ALVertex::ALVertex(QString str,int idx)
{
    this->data = str;
    this->id = idx;
    this->firstedge = nullptr;
    this->flag = 0;
}
