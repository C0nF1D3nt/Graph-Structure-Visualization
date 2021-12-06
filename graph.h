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

    QPair<Vertex*,ALVertex*> AddVertex(QString str);
    QPair<Edge*,QPair<ALEdge*,ALEdge*>> AddEdge(int id_i,int id_j,int w);
    bool DeleteVertex(int idx);
    bool DeleteEdge(int id_i,int id_j);
    void AdjmultiList_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void AdjmultiList_BFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void Stack_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v);
    void Initialflag();
    void DFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v);
    void BFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v);
    void PrimAlgorithmForMST(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v);
    QVector<int> DijkstraForMinPath(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v);

    int numVex;
    int numEdge;
    QVector<Vertex*> AdjmultiList;
    QVector<Edge*> EdgeList;
    QSet<QString> VertexNameSet;
    QVector<ALVertex*> AdjList;

    ~Graph();
};

#endif // GRAPH_H
