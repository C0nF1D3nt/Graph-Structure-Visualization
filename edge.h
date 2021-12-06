#ifndef EDGE_H
#define EDGE_H


class Edge  //adjacency multilist
{
public:
    Edge(int ivex = 0,int jvex = 0,int w = 0);

    int iVex;  //id of Vertex i
    Edge* iLink;  //next edge of i
    int jVex;  //id of Vertex j
    Edge* jLink;  //next edge of j
    int weight;
};

#endif // EDGE_H
