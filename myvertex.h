#ifndef MYVERTEX_H
#define MYVERTEX_H

#include <QGraphicsEllipseItem>
#include <QString>
#include <QPainter>
#include <QColor>
#include "alvertex.h"
#include "vertex.h"
#include <QVector>
#include <QEasingCurve>
#include <QTimeLine>

class MyVertex: public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum{ Type = UserType + 1 };
    MyVertex(QPointF Center,qreal R,QString Data = "");

    int id;
    QString data;

    QPointF center;
    qreal r;
    ALVertex* ALVex;
    Vertex* Vex;

    void StartSelectVexAnimation();
    void CancelSelectVexAnimation();

    int type() const{ return Type; }

};

#endif // MYVERTEX_H
