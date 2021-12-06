#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include "vertex.h"
#include "aledge.h"
#include "alvertex.h"
#include <QString>
#include <QVector>
#include <QSet>
#include <QQueue>
#include <QDebug>
#include <QStack>

class Graph
{
public:
    Graph();

    QPair<Vertex*,ALVertex*> AddVertex(QString str);  //添加结点
    QPair<Edge*,QPair<ALEdge*,ALEdge*>> AddEdge(int id_i,int id_j,int w);  //添加边
    bool DeleteVertex(int idx);  //删除结点
    bool DeleteEdge(int id_i,int id_j);  //删除边
    void AdjmultiList_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void AdjmultiList_BFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void Stack_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void Initialflag();  //访问标记初始化
    void DFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v); //建立深度优先生成树
    void BFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v); //建立广度优先生成树
    void PrimAlgorithmForMST(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v);  //Prim算法建立最小生成树
    QVector<int> DijkstraForMinPath(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v);  //Dijkstra算法求最短路径

    int numVex; //顶点的数目
    int numEdge;  //边的数目
    QVector<Vertex*> AdjmultiList;  //邻接多重表
    QVector<Edge*> EdgeList;  //边集列表
    QSet<QString> VertexNameSet;  //结点名字集合，结点的名字不允许重复
    QVector<ALVertex*> AdjList;  //邻接表

    ~Graph();
};

#endif // GRAPH_H
