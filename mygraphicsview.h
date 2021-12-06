#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include "myline.h"
#include "myvertex.h"
#include <QMouseEvent>
#include <QGraphicsScene>


class MyGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget* parent = nullptr);

    QGraphicsScene* MyScene;
    int SelectState = 0;
    int DrawLineState = 0;
    int MousePressState = 0;
    int DragVertexState = 0;
    MyVertex* Selectedvex = nullptr;
    MyLine* Selectedline = nullptr;
    MyVertex* DrawLineStartVex = nullptr;

    QVector<MyVertex*> AllVertex;
    QVector<MyLine*> AllLine;
    QVector<QGraphicsSimpleTextItem*> AllText;

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    void AddVex(QPointF center,qreal r = 13,QString data = "");
    void AddLine(MyVertex* vex1,MyVertex* vex2,int w = 1);
    void DeleteItem();
    MyVertex* FindVertex(Vertex* vex);
    MyLine* FindEdge(Edge* edge);
    MyVertex* FindALVertex(ALVertex* vex);
    MyLine* FindALEdge(ALEdge* edge);

    int SelectAnimationState = 0;

    QColor DefaultBrush = QColor(179,136,255);
    QColor SelectBrush = QColor(220,78,56);
    QColor DrawLineBrush = QColor(0,230,118);

    QPen DefaultPen = QPen(QBrush(QColor(144,164,174)),3);
    QPen SelectPen = QPen(QBrush(QColor(255,111,0,200)),5);

signals:
    void VertexAdded(MyVertex* vex);
    void LineAdded(MyLine* line);
    void DeleteLine(MyLine* line);
    void DeleteVertex(MyVertex* vex);
    void PresentInfo(QGraphicsItem*);
    void CloseVertexInfo();
    void CloseEdgeInfo();
    void mousePressed();

public slots:

};

#endif // MYGRAPHICSVIEW_H
