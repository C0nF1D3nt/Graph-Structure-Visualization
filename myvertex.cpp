#include "myvertex.h"

MyVertex::MyVertex(QPointF Center,qreal R,QString Data):
    QGraphicsEllipseItem(Center.x() - R, Center.y() - R, R * 2, R * 2)
{
    this->center = Center;
    this->r = R;
    this->ALVex = nullptr;
    this->Vex = nullptr;
    this->id = -1;
    this->data = Data;
    this->setPen(Qt::NoPen);
    this->setBrush(QBrush(QColor(179,136,255)));

}

void MyVertex::StartSelectVexAnimation(){
    QTimeLine* timeline = new QTimeLine(200,this);
    QEasingCurve curve = QEasingCurve::OutBounce;
    timeline->setFrameRange(0,100);
    connect(timeline,&QTimeLine::frameChanged,[=](int frame){
        qreal R = 13*(1-0.25*curve.valueForProgress(frame/100.0));
        this->setRect(center.x()-R,center.y()-R,2*R,2*R);
    });
    timeline->start();
}

void MyVertex::CancelSelectVexAnimation(){
    QTimeLine* timeline = new QTimeLine(200,this);
    QEasingCurve curve = QEasingCurve::OutBounce;
    timeline->setFrameRange(0,100);
    connect(timeline,&QTimeLine::frameChanged,[=](int frame){
        qreal R = 13*(0.75 + 0.25*curve.valueForProgress(frame/100.0));
        this->setRect(center.x()-R,center.y()-R,2*R,2*R);
    });
    timeline->start();
}
