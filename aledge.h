#ifndef ALEDGE_H
#define ALEDGE_H


class ALEdge  //邻接表
{
public:
    ALEdge(int idx,int w);

    int Vex;  //顶点下标
    int weight;  //顶点权重
    ALEdge* next; //下一条边
};

#endif // ALEDGE_H
