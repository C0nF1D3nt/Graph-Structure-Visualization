#ifndef ALEDGE_H
#define ALEDGE_H


class ALEdge  //Adjcency list
{
public:
    ALEdge(int idx,int w);

    int Vex;  //Vertex id
    int weight;  //edge weight
    ALEdge* next; //next edge
};

#endif // ALEDGE_H
