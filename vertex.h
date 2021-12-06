#ifndef VERTEX_H
#define VERTEX_H

#include <QString>
#include "edge.h"

class Vertex  //邻接多重表
{
public:
    Vertex(QString str = "",int idx = 0);

    QString data;  //顶点名字
    Edge* firstedge;  //第一条与该顶点连接的边
    int id;  //顶点下标
    int flag; //访问标记
};

#endif // VERTEX_H
