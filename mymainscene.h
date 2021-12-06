#ifndef MYMAINSCENE_H
#define MYMAINSCENE_H

#include <QWidget>
#include "mygraphicsview.h"
#include "myvertex.h"
#include "myline.h"
#include "aledge.h"
#include "alvertex.h"
#include "edge.h"
#include "graph.h"
#include "vertex.h"
#include <QPair>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QTimer>
#include <QTimeLine>
#include <QEasingCurve>
#include <QQueue>

class MyMainScene: public QWidget
{
    Q_OBJECT

public:
    Graph* g;
    MyGraphicsView* view;
    QQueue<QTimeLine*> animationQueue;
    QQueue<QGraphicsItem*> VisitQueue;
    QTimeLine* currentAnimation = nullptr;
    bool AnimationStarted = false;
    int ProgressType = -1;
    enum AniType{
        AMLDFS = 0,ALDFS,STACKDFS,AMLBFS,ALBFS,PRIM,DIJKSTRA
    };

public:
    MyMainScene(QWidget *parent = nullptr);

    void VisitedAnimation(QGraphicsItem* item);
    void StartAnimation();

signals:
    void VexNumChanged();
    void EdgeNumChanged();
    void AMLDFSStart();
    void ALDFSStart();
    void StackDFSStart();
    void AMLBFSStart();
    void ALBFSStart();
    void PrimStart();
    void DijkstraStart();
    void VertexVisited(MyVertex* vex);
    void EdgeVisited(MyLine* line);
    void AMLDFSEnd();
    void ALDFSEnd();
    void StackDFSEnd();
    void AMLBFSEnd();
    void ALBFSEnd();
    void PrimEnd();
    void DijkstraEnd();
    void ProgressInterrupt();
    void OutputMinPath(QVector<int> v);


public slots:
    void AddVertex(MyVertex* vex);
    void AddLine(MyLine* line);
    void DeleteLine(MyLine* line);
    void DeleteVertex(MyVertex* vex);
    void AMLDFSClicked();
    void ALDFSClicked();
    void StackDFSClicked();
    void AMLBFSClicked();
    void ALBFSClicked();
    void PrimClicked();
    void DijkstraClicked();

};

#endif // MYMAINSCENE_H
