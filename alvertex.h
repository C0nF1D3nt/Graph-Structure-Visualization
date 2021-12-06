#ifndef ALVERTEX_H
#define ALVERTEX_H

#include <QString>
#include "aledge.h"

class ALVertex  //邻接表
{
public:

    ALVertex(QString str = "",int idx = 0);

    QString data;  //顶点名字
    ALEdge* firstedge;  //第一条与该顶点连接的边
    int id;  //顶点下标
    int flag; //访问标记
};

#endif // ALVERTEX_H
