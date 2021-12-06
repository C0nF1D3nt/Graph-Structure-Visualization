#ifndef MYLINE_H
#define MYLINE_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QColor>
#include "aledge.h"
#include "edge.h"
#include "myvertex.h"

class MyLine: public QObject,public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum{ Type = UserType + 2 };
    MyLine(MyVertex* vex1,MyVertex* vex2,int w = 1);

    MyVertex* vex_i;
    MyVertex* vex_j;
    int id_i;
    int id_j;
    int weight;
    Edge* edge;
    ALEdge* ALedge_i;
    ALEdge* ALedge_j;

    int type() const{ return Type; }
};

#endif // MYLINE_H
