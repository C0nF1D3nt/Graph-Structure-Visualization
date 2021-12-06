#ifndef VERTEX_H
#define VERTEX_H

#include <QString>
#include "edge.h"

class Vertex  //adjacency multilist
{
public:
    Vertex(QString str = "",int idx = 0);

    QString data;  //Vertex name
    Edge* firstedge;  //first edge
    int id;  //Vertex id
    int flag; //Visited flag
};

#endif // VERTEX_H
