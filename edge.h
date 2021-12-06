#ifndef EDGE_H
#define EDGE_H


class Edge  //邻接多重表
{
public:
    Edge(int ivex = 0,int jvex = 0,int w = 0);

    int iVex;  //i顶点的id
    Edge* iLink;  //i顶点连接的下一条边
    int jVex;  //j顶点的id
    Edge* jLink;  //j顶点连接的下一条边
    int weight;  //边的权重
};

#endif // EDGE_H
