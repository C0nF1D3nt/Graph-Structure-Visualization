#ifndef ALVERTEX_H
#define ALVERTEX_H

#include <QString>
#include "aledge.h"

class ALVertex  //adjacency list
{
public:

    ALVertex(QString str = "",int idx = 0);

    QString data;  //vertex name
    ALEdge* firstedge;  //first edge
    int id;  //Vertex id
    int flag; //Visited flag
};

#endif // ALVERTEX_H
