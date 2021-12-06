#include "graph.h"
#include <QDebug>

Graph::Graph()
{
    this->numVex = 0;
    this->numEdge = 0;
    this->AdjmultiList.clear();
    this->AdjList.clear();
    this->EdgeList.clear();
    this->VertexNameSet.clear();
}

QPair<Vertex*,ALVertex*> Graph::AddVertex(QString str){
    if(this->VertexNameSet.find(str) != this->VertexNameSet.end()){
        return {nullptr,nullptr};  //the name is used
    }
    this->VertexNameSet.insert(str);
    int Id = this->AdjmultiList.size();
    Vertex* vex = new Vertex(str,Id);
    this->AdjmultiList.push_back(vex);
    ALVertex* ALvex = new ALVertex(str,Id);
    this->AdjList.push_back(ALvex);
    this->numVex++;
    return {vex,ALvex};
}

QPair<Edge*,QPair<ALEdge*,ALEdge*>> Graph::AddEdge(int id_i,int id_j,int w){
    for(int i = 0; i < this->EdgeList.size(); i++){
        if((id_i == EdgeList[i]->iVex && id_j == EdgeList[i]->jVex)
        || (id_i == EdgeList[i]->jVex && id_j == EdgeList[i]->iVex)
        || id_i >= this->AdjmultiList.size() || id_j >= this->AdjmultiList.size()){
            return {nullptr,{nullptr,nullptr}};  //the edge is existed or the vertex is not exist
        }
    }
    Edge* edge = new Edge(id_i,id_j,w);
    this->EdgeList.push_back(edge);
    edge->iLink = AdjmultiList[id_i]->firstedge;
    AdjmultiList[id_i]->firstedge = edge;
    edge->jLink = AdjmultiList[id_j]->firstedge;
    AdjmultiList[id_j]->firstedge = edge;
    ALEdge* edge1 = new ALEdge(id_j,w);
    edge1->next = AdjList[id_i]->firstedge;
    AdjList[id_i]->firstedge = edge1;
    ALEdge* edge2 = new ALEdge(id_i,w);
    edge2->next = AdjList[id_j]->firstedge;
    AdjList[id_j]->firstedge = edge2;
    this->numEdge++;
    return {edge,{edge1,edge2}};
}

bool Graph::DeleteVertex(int idx){
    if(idx >= AdjmultiList.size()){
        return false;  //not found
    }
    //delete from adjacency list
    ALEdge* p = AdjList[idx]->firstedge;
    while(p){
        ALEdge* q = p->next;
        ALEdge* temp = p;
        delete temp;
        p = q;
    }
    ALVertex* vex = AdjList[idx];
    AdjList.erase(AdjList.begin()+idx);
    delete vex;
    for(int i = 0; i < AdjList.size(); i++){
        if(i >= idx){
            AdjList[i]->id--;
        }
        ALEdge* edge = AdjList[i]->firstedge;
        if(edge && edge->Vex == idx){
            ALEdge* temp = edge;
            edge = edge->next;
            AdjList[i]->firstedge = edge;
            delete temp;
            while(edge){
                if(edge->Vex > idx){
                    edge->Vex--;
                }
                edge = edge->next;
            }
        }else if(edge && edge->Vex != idx){
            while(edge && edge->next){
                if(edge->Vex > idx){
                    edge->Vex--;
                }
                if(edge->next->Vex == idx){
                    ALEdge* temp = edge->next;
                    edge->next = temp->next;
                    delete temp;
                }
                edge = edge->next;
            }
            if(edge && edge->Vex > idx){
                edge->Vex--;
            }
        }
    }

    //delete from edge list
    for(int i = 0; i < EdgeList.size(); i++){
        if(EdgeList[i]->iVex == idx || EdgeList[i]->jVex == idx){
            EdgeList.erase(EdgeList.begin() + i);
            i--;
        }
    }

    numEdge = EdgeList.size();

    //delete from adjacency multilist
    for(int i = 0; i < AdjmultiList.size(); i++){
        if(i == idx){
            continue;
        }
        Edge* edge = AdjmultiList[i]->firstedge;
        if(!edge){
            continue;
        }
        if(edge->iVex == idx || edge->jVex == idx){
            if(edge->iVex == i){
                AdjmultiList[i]->firstedge = edge->iLink;
            }else{
                AdjmultiList[i]->firstedge = edge->jLink;
            }
        }else{
            while(edge){
                if(edge->iVex == i){
                    if(edge->iLink && (edge->iLink->iVex == idx || edge->iLink->jVex == idx)){
                        if(edge->iLink->iVex == i){
                            edge->iLink = edge->iLink->iLink;
                        }else{
                            edge->iLink = edge->iLink->jLink;
                        }
                    }
                    edge = edge->iLink;
                }else{
                    if(edge->jLink && (edge->jLink->iVex == idx || edge->jLink->jVex == idx)){
                        if(edge->jLink->iVex == i){
                            edge->jLink = edge->jLink->iLink;
                        }else{
                            edge->jLink = edge->jLink->jLink;
                        }
                    }
                    edge = edge->jLink;
                }
            }
        }
    }

    Edge* edge = AdjmultiList[idx]->firstedge;
    while(edge){
        Edge* temp = edge;
        if(edge->iVex == idx){
            edge = edge->iLink;
        }else{
            edge = edge->jLink;
        }
        delete temp;
    }
    Vertex* Vex = AdjmultiList[idx];
    AdjmultiList.erase(AdjmultiList.begin() + idx);
    delete Vex;

    for(int i = 0; i < AdjmultiList.size(); i++){
        if(AdjmultiList[i]->id > idx){
            if(AdjmultiList[i]->data == "V" + QString::number(AdjmultiList[i]->id)){
                AdjmultiList[i]->data = "V" + QString::number(AdjmultiList[i]->id-1);
            }
            AdjmultiList[i]->id--;
        }
        Edge* p = AdjmultiList[i]->firstedge;
        while(p){
            if(p->weight > 0){
                if(p->iVex > idx){
                    p->iVex--;
                }
                if(p->jVex > idx){
                    p->jVex--;
                }
            }
            p->weight = -p->weight;
            if(p->iVex == i){
                p = p->iLink;
            }else{
                p = p->jLink;
            }
        }
    }

    VertexNameSet.clear();
    for(int i = 0; i < AdjmultiList.size(); i++){
        VertexNameSet.insert(AdjmultiList[i]->data);
    }
    numVex--;
    return true;
}

bool Graph::DeleteEdge(int id_i,int id_j){
    int idx = -1;
    for(int i = 0; i < this->EdgeList.size(); i++){
        if((id_i == EdgeList[i]->iVex && id_j == EdgeList[i]->jVex) || (id_j == EdgeList[i]->iVex && id_i == EdgeList[i]->jVex)){
            idx = i;
            break;
        }
    }
    if(idx == -1){
        return false; //not found
    }
    Edge* edge = EdgeList[idx];
    this->EdgeList.erase(EdgeList.begin()+idx);

    //delete from adjacency list
    ALEdge* p = this->AdjList[id_i]->firstedge;
    if(p->Vex == id_j){
        ALEdge* temp = p;
        this->AdjList[id_i]->firstedge = p->next;
        delete temp;
    }else{
        while(p && p->next->Vex != id_j){
            p = p->next;
        }
        if(!p){
            return false;
        }
        ALEdge* temp = p->next;
        p->next = temp->next;
        delete temp;
    }
    p = this->AdjList[id_j]->firstedge;
    if(p->Vex == id_i){
        ALEdge* temp = p;
        this->AdjList[id_j]->firstedge = p->next;
        delete temp;
    }else{
        while(p->next && p->next->Vex != id_i){
            p = p->next;
        }
        if(!p->next){
            return false;
        }
        ALEdge* temp = p->next;
        p->next = temp->next;
        delete temp;
    }

    //delete from adjacency multilist
    Edge* q = this->AdjmultiList[id_i]->firstedge;
    if(q->iVex == id_i && q->jVex == id_j){
        AdjmultiList[id_i]->firstedge = q->iLink;
    }else if(q->iVex == id_j && q->jVex == id_i){
        AdjmultiList[id_i]->firstedge = q->jLink;
    }else{
        int flag = 0;
        while(q){
            if(q->iVex == id_i){
                if(q->iLink == edge){
                    flag = 1;
                    break;
                }
                q = q->iLink;
            }else{
                if(q->jLink == edge){
                    flag = 2;
                    break;
                }
                q = q->jLink;
            }
        }
        if(!q || !flag){
            return false;
        }
        if(flag == 1){
            if(q->iLink->iVex == id_i){
                q->iLink = q->iLink->iLink;
            }else{
                q->iLink = q->iLink->jLink;
            }
        }else if(flag == 2){
            if(q->jLink->iVex == id_i){
                q->jLink = q->jLink->iLink;
            }else{
                q->jLink = q->jLink->jLink;
            }
        }
    }

    q = this->AdjmultiList[id_j]->firstedge;
    if(q->iVex == id_i && q->jVex == id_j){
        AdjmultiList[id_j]->firstedge = q->jLink;
    }else if(q->iVex == id_j && q->jVex == id_i){
        AdjmultiList[id_j]->firstedge = q->iLink;
    }else{
        int flag = 0;
        while(q){
            if(q->iVex == id_j){
                if(q->iLink == edge){
                    flag = 1;
                    break;
                }
                q = q->iLink;
            }else{
                if(q->jLink == edge){
                    flag = 2;
                    break;
                }
                q = q->jLink;
            }
        }
        if(!q || !flag){
            return false;
        }
        if(flag == 1){
            if(q->iLink->iVex == id_j){
                q->iLink = q->iLink->iLink;
            }else{
                q->iLink = q->iLink->jLink;
            }
        }else if(flag == 2){
            if(q->jLink->iVex == id_j){
                q->jLink = q->jLink->iLink;
            }else{
                q->jLink = q->jLink->jLink;
            }
        }
    }
    delete edge;
    this->numEdge--;
    return true;
}

void Graph::Initialflag(){
    for(int i = 0; i < this->AdjmultiList.size(); i++){
        this->AdjmultiList[i]->flag = 0;
    }
    for(int i = 0; i < this->AdjList.size(); i++){
        this->AdjList[i]->flag = 0;
    }
}

void Graph::AdjmultiList_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v){
    vex->flag = 1;
    v.first.push_back(vex);
    Edge* p = vex->firstedge;
    while(p){
        if(p->iVex == vex->id){
            if(AdjmultiList[p->jVex]->flag == 0){
                v.second.push_back(p);
                AdjmultiList_DFS(AdjmultiList[p->jVex],v);
            }
            p = p->iLink;
        }else{
            if(AdjmultiList[p->iVex]->flag == 0){
                v.second.push_back(p);
                AdjmultiList_DFS(AdjmultiList[p->iVex],v);
            }
            p = p->jLink;
        }
    }
}

void Graph::AdjmultiList_BFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v){
    QQueue<Vertex*> q;
    q.push_back(vex);
    vex->flag = 1;
    while(!q.empty()){
        Vertex* p = q.front();
        q.pop_front();
        int Id = p->id;
        v.first.push_back(p);
        Edge* edge = p->firstedge;
        while(edge){
           if(edge->iVex == Id){
               if(this->AdjmultiList[edge->jVex]->flag == 0){
                   AdjmultiList[edge->jVex]->flag = 1;
                   v.second.push_back(edge);
                   q.push_back(AdjmultiList[edge->jVex]);
               }
               edge = edge->iLink;
           }else{
               if(this->AdjmultiList[edge->iVex]->flag == 0){
                   AdjmultiList[edge->iVex]->flag = 1;
                   v.second.push_back(edge);
                   q.push_back(AdjmultiList[edge->iVex]);
               }
               edge = edge->jLink;
           }
        }
    }
}

void Graph::Stack_DFS(Vertex* vex,QPair<QVector<Vertex*>,QVector<Edge*>>& v){
    QStack<Vertex*> stk;
    QStack<Edge*> Edge_stk;
    stk.push(vex);
    while(!stk.empty()){
        Vertex* p = stk.top();
        stk.pop();
        if(p->flag != 1){
            v.first.push_back(p);
        }
        if(!Edge_stk.empty()){
            Edge* q = Edge_stk.top();
            Edge_stk.pop();
            if(p->flag != 1){
                v.second.push_back(q);
            }
        }
        p->flag = 1;
        int Id = p->id;
        Edge* edge = p->firstedge;
        while(edge){
            if(edge->iVex == Id){
                if(this->AdjmultiList[edge->jVex]->flag == 0){
                    stk.push(this->AdjmultiList[edge->jVex]);
                    Edge_stk.push(edge);
                }
                edge = edge->iLink;
            }else{
                if(this->AdjmultiList[edge->iVex]->flag == 0){
                    stk.push(this->AdjmultiList[edge->iVex]);
                    Edge_stk.push(edge);
                }
                edge = edge->jLink;
            }
        }
    }
}

void Graph::DFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v){
    vex->flag = 1;
    v.first.push_back(vex);
    ALEdge* p = vex->firstedge;
    while(p){
        if(AdjList[p->Vex]->flag == 0){
            v.second.push_back(p);
            DFSToCreateSpanningTree(AdjList[p->Vex],v);
        }
        p = p->next;
    }
}

void Graph::BFSToCreateSpanningTree(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALEdge*>>& v){
    QQueue<ALVertex*> q;
    q.push_back(vex);
    vex->flag = 1;
    while(!q.empty()){
        ALVertex* p = q.front();
        q.pop_front();
        v.first.push_back(p);
        ALEdge* edge = p->firstedge;
        while(edge){
            if(AdjList[edge->Vex]->flag == 0){
                v.second.push_back(edge);
                AdjList[edge->Vex]->flag = 1;
                q.push_back(AdjList[edge->Vex]);
            }
            edge = edge->next;
        }
    }
}

void Graph::PrimAlgorithmForMST(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v){
    QVector<int> MinCost(this->numVex,INT_MAX);
    int idx = -1;
    for(int i = 0; i < AdjList.size(); i++){
        if(AdjList[i] == vex){
            idx = i;
            break;
        }
    }
    QVector<int> AdjVex(this->numVex,idx);
    ALEdge* p = AdjList[idx]->firstedge;
    while(p){
        MinCost[p->Vex] = p->weight;
        p = p->next;
    }
    MinCost[idx] = 0;
    AdjList[idx]->flag = 1;
    for(int i = 1; i < numVex; i++){
        int dmin = INT_MAX;
        int idx = -1;
        for(int j = 0; j < numVex; j++){
            if(MinCost[j] && MinCost[j] < dmin){
                dmin = MinCost[j];
                idx = j;
            }
        }
        if(idx == -1){
            return;  //exist non-connected nodes
        }
        v.first.push_back(AdjList[AdjVex[idx]]);
        v.second.push_back(AdjList[idx]);
        MinCost[idx] = 0;
        AdjList[idx]->flag = 1;
        ALEdge* p = AdjList[idx]->firstedge;
        while(p){
            if(MinCost[p->Vex] && p->weight < MinCost[p->Vex]){
                MinCost[p->Vex] = p->weight;
                AdjVex[p->Vex] = idx;
            }
            p = p->next;
        }
    }
}

QVector<int> Graph::DijkstraForMinPath(ALVertex* vex,QPair<QVector<ALVertex*>,QVector<ALVertex*>>& v){
    QVector<int> flag(numVex,0);
    int idx = -1;
    for(int i = 0; i < AdjList.size(); i++){
        if(AdjList[i] == vex){
            idx = i;
            break;
        }
    }
    QVector<int> PathVex(numVex,idx);
    QVector<int> MinPath(numVex,INT_MAX);
    ALEdge* p = AdjList[idx]->firstedge;
    while(p){
        MinPath[p->Vex] = p->weight;
        p = p->next;
    }
    MinPath[idx] = 0;
    flag[idx] = 1;
    AdjList[idx]->flag = 1;
    for(int i = 1; i < numVex; i++){
        int dmin = INT_MAX;
        int idx = -1;
        for(int j = 0; j < numVex; j++){
            if(!flag[j] && MinPath[j] < dmin){
                dmin = MinPath[j];
                idx = j;
            }
        }
        if(idx == -1){  //exist non-connected nodes
            return MinPath;
        }
        flag[idx] = 1;
        AdjList[idx]->flag = 1;
        v.first.push_back(AdjList[PathVex[idx]]);
        v.second.push_back(AdjList[idx]);
        ALEdge* p = AdjList[idx]->firstedge;
        while(p){
            if(!flag[p->Vex] && dmin + p->weight < MinPath[p->Vex]){
                MinPath[p->Vex] = dmin + p->weight;
                PathVex[p->Vex] = idx;
            }
            p = p->next;
        }
    }
    return MinPath;
}


Graph::~Graph(){
    for(int i = 0; i < this->EdgeList.size(); i++){
        delete this->EdgeList[i];
    }
    for(int i = 0; i < this->AdjmultiList.size(); i++){
        delete this->AdjmultiList[i];
    }
    for(int i = 0; i < this->AdjList.size(); i++){
        ALEdge* edge = this->AdjList[i]->firstedge;
        while(edge){
            ALEdge* p = edge->next;
            ALEdge* q = edge;
            delete q;
            edge = p;
        }
        delete this->AdjList[i];
    }
}
