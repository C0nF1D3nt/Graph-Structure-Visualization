#include "myline.h"

MyLine::MyLine(MyVertex* vex1,MyVertex* vex2,int w):
    QGraphicsLineItem(vex1->center.x(),vex1->center.y(),vex2->center.x(),vex2->center.y())
{
    vex_i = vex1;
    vex_j = vex2;
    id_i = vex1->id;
    id_j = vex2->id;
    weight = w;
    this->ALedge_i = nullptr;
    this->ALedge_j = nullptr;
    this->edge = nullptr;
    this->setPen(QPen(QBrush(QColor(144,164,174)),3));
}
