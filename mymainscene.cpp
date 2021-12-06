#include "mymainscene.h"
#include <QVBoxLayout>

MyMainScene::MyMainScene(QWidget *parent): QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    view = new MyGraphicsView(this);
    view->setSceneRect(view->rect());
    layout->addWidget(view);
    g = new Graph();
    connect(view,SIGNAL(VertexAdded(MyVertex*)), this, SLOT(AddVertex(MyVertex*)));
    connect(view,SIGNAL(LineAdded(MyLine*)), this, SLOT(AddLine(MyLine*)));
    connect(view,SIGNAL(DeleteLine(MyLine*)),this,SLOT(DeleteLine(MyLine*)));
    connect(view,SIGNAL(DeleteVertex(MyVertex*)),this,SLOT(DeleteVertex(MyVertex*)));
    connect(view,&MyGraphicsView::mousePressed,[=](){
        if(!animationQueue.empty() || AnimationStarted){
            animationQueue.clear();
            currentAnimation->stop();
            currentAnimation = nullptr;
            for(int i = 0; i < view->AllVertex.size(); i++){
                view->AllVertex[i]->setBrush(view->DefaultBrush);
            }
            for(int i = 0; i < view->AllLine.size(); i++){
                view->AllLine[i]->setPen(view->DefaultPen);
            }
            AnimationStarted = false;
            emit ProgressInterrupt();
            ProgressType = -1;
        }
    });
}

void MyMainScene::AddVertex(MyVertex* vex){
    int Id = g->AdjmultiList.size();
    QString default_name = "V" + QString::number(Id);
    if(vex->data == ""){
        QPair<Vertex*,ALVertex*> Vex = g->AddVertex(default_name);
        vex->id = Id;
        vex->data = default_name;
        vex->Vex = Vex.first;
        vex->ALVex = Vex.second;
    }else{
        QPair<Vertex*,ALVertex*> Vex = g->AddVertex(vex->data);
        vex->id = Id;
        vex->Vex = Vex.first;
        vex->ALVex = Vex.second;
    }
    emit VexNumChanged();
}

void MyMainScene::AddLine(MyLine* line){
    QPair<Edge*,QPair<ALEdge*,ALEdge*>> Line = g->AddEdge(line->id_i,line->id_j,line->weight);
    line->edge = Line.first;
    line->ALedge_i = Line.second.first;
    line->ALedge_j = Line.second.second;
    emit EdgeNumChanged();
}

void MyMainScene::DeleteLine(MyLine* line){
    g->DeleteEdge(line->id_i,line->id_j);
    emit EdgeNumChanged();
}

void MyMainScene::DeleteVertex(MyVertex* vex){
    g->DeleteVertex(vex->id);
    emit VexNumChanged();
    emit EdgeNumChanged();
}

void MyMainScene::AMLDFSClicked(){
    ProgressType = AMLDFS;
    emit AMLDFSStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<Vertex*>,QVector<Edge*>> v;
        g->AdjmultiList_DFS(view->Selectedvex->Vex,v);
        for(int i = 0; i < v.first.size(); i++){
            if(i > 0 && i <= v.second.size()){
                VisitedAnimation(view->FindEdge(v.second[i-1]));
            }
            VisitedAnimation(view->FindVertex(v.first[i]));
        }
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->AdjmultiList_DFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }else{       
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->AdjmultiList_DFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::VisitedAnimation(QGraphicsItem* item){
    QTimeLine* timeline = new QTimeLine(800,this);
    timeline->setFrameRange(0,200);
    VisitQueue.push_back(item);
    if(item->type() == MyVertex::Type){
        MyVertex* vex = qgraphicsitem_cast<MyVertex*>(item);        
        //emit VertexVisited(vex);
        connect(timeline,&QTimeLine::frameChanged,[=](int frame){
            QEasingCurve curve = QEasingCurve::InOutCirc;
            int alpha = 50 + 205.0*curve.valueForProgress(frame/200.0);
            vex->setBrush(QBrush(QColor(255,111,0,alpha)));
        });
    }else if(item->type() == MyLine::Type){
        MyLine* line = qgraphicsitem_cast<MyLine*>(item);
        //emit EdgeVisited(line);
        connect(timeline,&QTimeLine::frameChanged,[=](int frame){
            QEasingCurve curve = QEasingCurve::InOutCirc;
            int alpha = 50 + 205.0*curve.valueForProgress(frame/200.0);
            line->setPen(QPen(QBrush(QColor(255,111,0,alpha)),3));
        });
    }
    animationQueue.push_back(timeline);
}

void MyMainScene::StartAnimation(){
    if(!animationQueue.empty()){
        QTimeLine* timeline = animationQueue.front();
        animationQueue.pop_front();
        QGraphicsItem* item = VisitQueue.front();
        VisitQueue.pop_front();
        if(item->type() == MyVertex::Type){
            MyVertex* vex = qgraphicsitem_cast<MyVertex*>(item);
            emit VertexVisited(vex);
        }else if(item->type() == MyLine::Type){
            MyLine* line = qgraphicsitem_cast<MyLine*>(item);
            emit EdgeVisited(line);
        }
        currentAnimation = timeline;
        connect(timeline,&QTimeLine::finished,[=](){
            StartAnimation();
        });
        timeline->start();
        AnimationStarted = true;
    }else{
        switch(ProgressType){
        case AMLDFS:
            emit AMLDFSEnd();
            break;
        case ALDFS:
            emit ALDFSEnd();
            break;
        case STACKDFS:
            emit StackDFSEnd();
            break;
        case AMLBFS:
            emit AMLBFSEnd();
            break;
        case ALBFS:
            emit ALBFSEnd();
            break;
        case PRIM:
            emit PrimEnd();
            break;
        case DIJKSTRA:
            emit DijkstraEnd();
            break;
        }
    }
}

void MyMainScene::ALDFSClicked(){
    ProgressType = ALDFS;
    emit ALDFSStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
        g->DFSToCreateSpanningTree(view->Selectedvex->ALVex,v);
        for(int i = 0; i < v.first.size(); i++){
            if(i > 0 && i <= v.second.size()){
                VisitedAnimation(view->FindALEdge(v.second[i-1]));
            }
            VisitedAnimation(view->FindALVertex(v.first[i]));
        }
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
                g->DFSToCreateSpanningTree(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindALEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindALVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }else{
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
                g->DFSToCreateSpanningTree(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindALEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindALVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::StackDFSClicked(){
    ProgressType = STACKDFS;
    emit StackDFSStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<Vertex*>,QVector<Edge*>> v;
        g->Stack_DFS(view->Selectedvex->Vex,v);
        for(int i = 0; i < v.first.size(); i++){
            if(i > 0 && i <= v.second.size()){
                VisitedAnimation(view->FindEdge(v.second[i-1]));
            }
            VisitedAnimation(view->FindVertex(v.first[i]));
        }
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->Stack_DFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }else{
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->Stack_DFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::AMLBFSClicked(){
    ProgressType = AMLBFS;
    emit AMLBFSStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<Vertex*>,QVector<Edge*>> v;
        g->AdjmultiList_BFS(view->Selectedvex->Vex,v);
        for(int i = 0; i < v.first.size(); i++){
            if(i > 0 && i <= v.second.size()){
                VisitedAnimation(view->FindEdge(v.second[i-1]));
            }
            VisitedAnimation(view->FindVertex(v.first[i]));
        }
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->AdjmultiList_BFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }else{
        for(int i = 0; i < g->AdjmultiList.size(); i++){
            if(g->AdjmultiList[i]->flag == 0){
                QPair<QVector<Vertex*>,QVector<Edge*>> v;
                g->AdjmultiList_BFS(g->AdjmultiList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::ALBFSClicked(){
    ProgressType = ALBFS;
    emit ALBFSStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
        g->BFSToCreateSpanningTree(view->Selectedvex->ALVex,v);
        for(int i = 0; i < v.first.size(); i++){
            if(i > 0 && i <= v.second.size()){
                VisitedAnimation(view->FindALEdge(v.second[i-1]));
            }
            VisitedAnimation(view->FindALVertex(v.first[i]));
        }
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
                g->BFSToCreateSpanningTree(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindALEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindALVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }else{
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALEdge*>> v;
                g->BFSToCreateSpanningTree(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        VisitedAnimation(view->FindALEdge(v.second[i-1]));
                    }
                    VisitedAnimation(view->FindALVertex(v.first[i]));
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::PrimClicked(){
    ProgressType = PRIM;
    emit PrimStart();
    g->Initialflag();
    for(int i = 0; i < view->AllVertex.size(); i++){
        view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
    }
    for(int i = 0; i < view->AllLine.size(); i++){
        view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
    }
    if(view->Selectedvex != nullptr){
        QPair<QVector<ALVertex*>,QVector<ALVertex*>> v;
        g->PrimAlgorithmForMST(view->Selectedvex->ALVex,v);
        VisitedAnimation(view->Selectedvex);
        for(int i = 0; i < v.first.size(); i++){
            int idx = -1;
            for(int k = 0; k < view->AllLine.size(); k++){
                if((view->AllLine[k]->vex_i->ALVex == v.first[i] && view->AllLine[k]->vex_j->ALVex == v.second[i])
                        || (view->AllLine[k]->vex_j->ALVex == v.first[i] && view->AllLine[k]->vex_i->ALVex == v.second[i])){
                    idx = k;
                    break;
                }
            }
            VisitedAnimation(view->AllLine[idx]);
            if(v.first[i]->flag == 0){
                VisitedAnimation(view->FindALVertex(v.first[i]));
            }else{
                VisitedAnimation(view->FindALVertex(v.second[i]));
            }
        }
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALVertex*>> v;
                g->PrimAlgorithmForMST(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        int idx = -1;
                        for(int k = 0; k < view->AllLine.size(); k++){
                            if((view->AllLine[k]->vex_i->ALVex == v.first[i] && view->AllLine[k]->vex_j->ALVex == v.second[i])
                                    || (view->AllLine[k]->vex_j->ALVex == v.first[i] && view->AllLine[k]->vex_i->ALVex == v.second[i])){
                                idx = k;
                                break;
                            }
                        }
                        qDebug() << view->AllLine[idx]->id_i << view->AllLine[idx]->id_j;
                        VisitedAnimation(view->AllLine[idx]);
                    }
                    if(v.first[i]->flag == 0){
                        VisitedAnimation(view->FindALVertex(v.first[i]));
                    }else{
                        VisitedAnimation(view->FindALVertex(v.second[i]));
                    }
                }
            }
        }
        StartAnimation();
    }else{
        for(int i = 0; i < g->AdjList.size(); i++){
            if(g->AdjList[i]->flag == 0){
                QPair<QVector<ALVertex*>,QVector<ALVertex*>> v;
                g->PrimAlgorithmForMST(g->AdjList[i],v);
                for(int i = 0; i < v.first.size(); i++){
                    if(i > 0 && i <= v.second.size()){
                        int idx = -1;
                        for(int k = 0; k < view->AllLine.size(); k++){
                            if((view->AllLine[k]->vex_i->ALVex == v.first[i] && view->AllLine[k]->vex_j->ALVex == v.second[i])
                                    || (view->AllLine[k]->vex_j->ALVex == v.first[i] && view->AllLine[k]->vex_i->ALVex == v.second[i])){
                                idx = k;
                                break;
                            }
                        }
                        VisitedAnimation(view->AllLine[idx]);
                        qDebug() << view->AllLine[idx]->id_i << view->AllLine[idx]->id_j;
                    }
                    if(v.first[i]->flag == 0){
                        VisitedAnimation(view->FindALVertex(v.first[i]));
                    }else{
                        VisitedAnimation(view->FindALVertex(v.second[i]));
                    }
                }
            }
        }
        StartAnimation();
    }
}

void MyMainScene::DijkstraClicked(){
    g->Initialflag();
    if(view->Selectedvex != nullptr){
        emit DijkstraStart();
        ProgressType = DIJKSTRA;
        QVector<int> minpath;
        for(int i = 0; i < view->AllVertex.size(); i++){
            view->AllVertex[i]->setBrush(QBrush(QColor(179,136,255,50)));
        }
        for(int i = 0; i < view->AllLine.size(); i++){
            view->AllLine[i]->setPen(QPen(QBrush(QColor(144,164,174,50)),3));
        }
        QPair<QVector<ALVertex*>,QVector<ALVertex*>> v;
        minpath = g->DijkstraForMinPath(view->Selectedvex->ALVex,v);
        VisitedAnimation(view->Selectedvex);
        for(int i = 0; i < v.first.size(); i++){
            int idx = -1;
            for(int k = 0; k < view->AllLine.size(); k++){
                if((view->AllLine[k]->vex_i->ALVex == v.first[i] && view->AllLine[k]->vex_j->ALVex == v.second[i])
                        || (view->AllLine[k]->vex_j->ALVex == v.first[i] && view->AllLine[k]->vex_i->ALVex == v.second[i])){
                    idx = k;
                    break;
                }
            }
            VisitedAnimation(view->AllLine[idx]);
            if(v.first[i]->flag == 0){
                VisitedAnimation(view->FindALVertex(v.first[i]));
            }else{
                VisitedAnimation(view->FindALVertex(v.second[i]));
            }
        }
        StartAnimation();
        connect(this,&MyMainScene::DijkstraEnd,[=](){
            emit OutputMinPath(minpath);
        });
    }
}

