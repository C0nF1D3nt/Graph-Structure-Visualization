#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget* parent):
    QGraphicsView(parent)
{
    this->setMouseTracking(true);
    MyScene = new QGraphicsScene();
    this->setScene(MyScene);
    this->setSceneRect(this->rect());
    this->setRenderHint(QPainter::Antialiasing);
}

void MyGraphicsView::mousePressEvent(QMouseEvent* event){
    emit mousePressed();
    QList<QGraphicsItem*> list = scene()->items(this->mapToScene(event->pos()).toPoint());   
    if(event->button() == Qt::LeftButton){
        DrawLineState = 0;
        if(DrawLineStartVex != nullptr){
            DrawLineStartVex->setBrush(DefaultBrush);
            DrawLineStartVex = nullptr;
        }
        if(list.size() != 0){
            //selected an item
            if(SelectState == 0){
                //have not selected an item
                if(list[0]->type() == MyVertex::Type){
                    //selected a vertex
                    MyVertex* vex = qgraphicsitem_cast<MyVertex*>(list[0]);
                    vex->setBrush(SelectBrush);
                    SelectState = 1;
                    Selectedvex = vex;
                }else if(list[0]->type() == MyLine::Type){
                    //selected a line
                    MyLine* line = qgraphicsitem_cast<MyLine*>(list[0]);
                    line->setPen(SelectPen);
                    SelectState = 1;
                    Selectedline = line;
                }
            }else{
                //have selected an item
                if(Selectedvex != nullptr){
                    Selectedvex->setBrush(DefaultBrush);
                    if(list[0]->type() == MyVertex::Type){
                        MyVertex* vex = qgraphicsitem_cast<MyVertex*>(list[0]);
                        vex->setBrush(SelectBrush);
                        Selectedvex = vex;
                        Selectedline = nullptr;
                    }else if(list[0]->type() == MyLine::Type){
                        MyLine* line = qgraphicsitem_cast<MyLine*>(list[0]);
                        line->setPen(SelectPen);
                        Selectedline = line;
                        Selectedvex = nullptr;
                    }
                }else if(Selectedline != nullptr){
                    Selectedline->setPen(DefaultPen);
                    if(list[0]->type() == MyVertex::Type){
                        MyVertex* vex = qgraphicsitem_cast<MyVertex*>(list[0]);
                        vex->setBrush(SelectBrush);
                        Selectedvex = vex;
                        Selectedline = nullptr;
                    }else if(list[0]->type() == MyLine::Type){
                        MyLine* line = qgraphicsitem_cast<MyLine*>(list[0]);
                        line->setPen(SelectPen);
                        Selectedline = line;
                        Selectedvex = nullptr;
                    }
                }
            }
        }
    }else{
        SelectState = 0;
        if(Selectedvex != nullptr){
            Selectedvex->setBrush(DefaultBrush);
            Selectedvex = nullptr;
        }
        if(Selectedline != nullptr){
            Selectedline->setPen(DefaultPen);
            Selectedline = nullptr;
        }
    }
    if(list.size() > 0 && Selectedvex != nullptr && list[0] == Selectedvex){
        MousePressState = 1;
    }
    if(Selectedvex != nullptr && list.size() > 0 && list[0] == Selectedvex){
        SelectAnimationState = 1;
        Selectedvex->StartSelectVexAnimation();
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent* event){
    QList<QGraphicsItem*> list = scene()->items(this->mapToScene(event->pos()).toPoint());
    if(Selectedvex != nullptr && SelectAnimationState == 1){
        Selectedvex->CancelSelectVexAnimation();
        SelectAnimationState = 0;
    }
    MousePressState = 0;
    if(DragVertexState == 1){
        if(Selectedvex != nullptr){
            SelectState = 0;
            Selectedvex->setBrush(DefaultBrush);
            Selectedvex = nullptr;
        }
    }

    if(event->button() == Qt::LeftButton){
        if(list.size() == 0 && SelectState == 0 && DragVertexState == 0){
            AddVex(mapToScene(event->pos()));
        }else if(list.size() == 0 && (SelectState == 1 || DrawLineState == 1)){
            //cancel select vertex/line or draw line
            if(SelectState == 1){
                //cancel select                
                if(Selectedvex != nullptr){
                    Selectedvex->setBrush(DefaultBrush);
                }
                if(Selectedline != nullptr){
                    Selectedline->setPen(DefaultPen);
                }
            }
            if(DrawLineState == 1){
                //cancel draw a line
                DrawLineStartVex->setBrush(DefaultBrush);
            }
            SelectState = 0;
            DrawLineState = 0;
            Selectedvex = nullptr;
            Selectedline = nullptr;
            DrawLineStartVex = nullptr;
        }else if(list.size() > 0 && SelectState == 1){
            if(Selectedvex != nullptr){
                emit CloseEdgeInfo();
                emit PresentInfo(Selectedvex);
            }else if(Selectedline != nullptr){
                emit CloseVertexInfo();
                emit PresentInfo(Selectedline);
            }
        }
    }else{
        if(list.size() > 0 && DrawLineState == 0){
            if(list[0]->type() == MyVertex::Type){
                DrawLineState = 1;
                MyVertex* vex = qgraphicsitem_cast<MyVertex*>(list[0]);
                vex->setBrush(DrawLineBrush);
                DrawLineStartVex = vex;
            }
        }else if(list.size() > 0 && DrawLineState == 1){
            if(list[0]->type() == MyVertex::Type){
                MyVertex* vex = qgraphicsitem_cast<MyVertex*>(list[0]);
                DrawLineStartVex->setBrush(DefaultBrush);
                AddLine(DrawLineStartVex,vex);
                DrawLineState = 0;
                DrawLineStartVex = nullptr;
            }else{
                DrawLineStartVex->setBrush(DefaultBrush);
                DrawLineState = 0;
                DrawLineStartVex = nullptr;
            }
        }else if(list.size() == 0 && (SelectState == 1 || DrawLineState == 1)){
            //cancel select vertex/line or draw line
            if(SelectState == 1){
                //cancel select
                if(Selectedvex != nullptr){
                    Selectedvex->setBrush(DefaultBrush);
                }
                if(Selectedline != nullptr){
                    Selectedline->setPen(DefaultPen);
                }
            }
            if(DrawLineState == 1){
                //cancel draw a line
                DrawLineStartVex->setBrush(DefaultBrush);
            }
            SelectState = 0;
            DrawLineState = 0;
            Selectedvex = nullptr;
            Selectedline = nullptr;
            DrawLineStartVex = nullptr;
        }
    }
    DragVertexState = 0;
    if(SelectState == 0){
        emit CloseVertexInfo();
        emit CloseEdgeInfo();
    }else{
        if(Selectedvex != nullptr){
            emit PresentInfo(Selectedvex);
        }else if(Selectedline != nullptr){
            emit PresentInfo(Selectedline);
        }
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent* event){
    QList<QGraphicsItem*> list = scene()->items(this->mapToScene(event->pos()).toPoint());
    if(Selectedvex != nullptr && MousePressState == 1){
        QPointF point = Selectedvex->scenePos() + Selectedvex->rect().center();
        QPointF dp = mapToScene(event->pos()) - point;
        Selectedvex->setRect(Selectedvex->rect().x() + dp.x(), Selectedvex->rect().y() + dp.y(), Selectedvex->rect().width(), Selectedvex->rect().height());
        Selectedvex->center += dp;
        for(int i = 0; i < AllLine.size(); i++){
            if(AllLine[i]->vex_i == Selectedvex || AllLine[i]->vex_j == Selectedvex){
                if(AllLine[i]->vex_i == Selectedvex){
                    QPointF point2 = AllLine[i]->vex_j->scenePos() + AllLine[i]->vex_j->rect().center();
                    AllLine[i]->setLine(point.x(),point.y(),point2.x(),point2.y());
                }else{
                    QPointF point2 = AllLine[i]->vex_i->scenePos() + AllLine[i]->vex_i->rect().center();
                    AllLine[i]->setLine(point.x(),point.y(),point2.x(),point2.y());
                }
            }
        }
        DragVertexState = 1;
    }

//    //present info
//    if(MousePressState == 0 && list.size() > 0){
//        AllText.clear();
//        for(int i = 0; i < AllText.size(); i++){
//           AllText[i]->show();
//        }
//    }else if(MousePressState == 0 && list.size() == 0){
//        for(int i = 0; i < AllText.size(); i++){
//            AllText[i]->hide();
//        }
//    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent* event){
    switch (event->key()) {
        case Qt::Key_Backspace:
            DeleteItem();
            emit CloseVertexInfo();
            emit CloseEdgeInfo();
            break;
    }
}

void MyGraphicsView::AddVex(QPointF center, qreal r,QString data){
    MyVertex* vex = new MyVertex(center,r,data);
    this->MyScene->addItem(vex);
    this->AllVertex.push_back(vex);
//    QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(vex);
//    AllText.push_back(text);
//    text->setText(vex->data);
//    text->setPos(vex->center.x()-15,vex->center.y()+15);
//    text->setFont(QFont("Comic Sans MS",14));
    emit VertexAdded(vex);
}

void MyGraphicsView::AddLine(MyVertex* vex1,MyVertex* vex2,int w){
    if(vex1 == vex2){
        return;
    }
    for(int i = 0; i < AllLine.size(); i++){
        if((AllLine[i]->id_i == vex1->id && AllLine[i]->id_j == vex2->id)
           || (AllLine[i]->id_j == vex1->id && AllLine[i]->id_i == vex2->id)){
            return;
        }
    }
    MyLine* line = new MyLine(vex1,vex2,w);
    line->setZValue(-1);
    this->MyScene->addItem(line);
    this->AllLine.push_back(line);
    emit LineAdded(line);
}

void MyGraphicsView::DeleteItem(){
    if(SelectState == 1){
        if(Selectedvex != nullptr){
            emit DeleteVertex(Selectedvex);
            this->MyScene->removeItem(Selectedvex);
            int idx = -1;
            for(int i = 0; i < AllVertex.size(); i++){
                if(AllVertex[i] == Selectedvex){
                    idx = i;
                    break;
                }
            }
            AllVertex.removeAt(idx);
            int i = 0;
            while(i < AllLine.size()){
                if(AllLine[i]->vex_i == Selectedvex || AllLine[i]->vex_j == Selectedvex){
                    this->MyScene->removeItem(AllLine[i]);
                    AllLine.removeAt(i);
                }else{
                    i++;
                }
            }
            for(int i = 0; i < AllVertex.size(); i++){
                if(AllVertex[i]->id > idx){
                    if(AllVertex[i]->data == "V" + QString::number(AllVertex[i]->id)){
                        AllVertex[i]->data = "V" + QString::number(AllVertex[i]->id-1);
                    }
                    AllVertex[i]->id--;
                }
            }
            for(int i = 0; i < AllLine.size(); i++){
                AllLine[i]->id_i = AllLine[i]->vex_i->id;
                AllLine[i]->id_j = AllLine[i]->vex_j->id;
            }
        }else if(Selectedline != nullptr){
            emit DeleteLine(Selectedline);
            this->MyScene->removeItem(Selectedline);
            int idx = -1;
            for(int i = 0; i < AllLine.size(); i++){
                if(AllLine[i] == Selectedline){
                    idx = i;
                    break;
                }
            }
            AllLine.removeAt(idx);
        }
        SelectState = 0;
        Selectedline = nullptr;
        Selectedvex = nullptr;
    }
}

MyVertex* MyGraphicsView::FindVertex(Vertex* vex){
    for(int i = 0; i < AllVertex.size(); i++){
        if(AllVertex[i]->Vex == vex){
            return AllVertex[i];
        }
    }
    return nullptr;
}

MyLine* MyGraphicsView::FindEdge(Edge* edge){
    for(int i = 0; i < AllLine.size(); i++){
        if(AllLine[i]->edge == edge){
            return AllLine[i];
        }
    }
    return nullptr;
}

MyVertex* MyGraphicsView::FindALVertex(ALVertex* vex){
    for(int i = 0; i < AllVertex.size(); i++){
        if(AllVertex[i]->ALVex == vex){
            return AllVertex[i];
        }
    }
    return nullptr;
}

MyLine* MyGraphicsView::FindALEdge(ALEdge* edge){
    for(int i = 0; i < AllLine.size(); i++){
        if(AllLine[i]->ALedge_i == edge || AllLine[i]->ALedge_j == edge){
            return AllLine[i];
        }
    }
    return nullptr;
}

