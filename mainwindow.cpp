#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1200,800);
    this->setWindowTitle("Graph Structure");
    this->setStyleSheet("QMainWindow{background-color:#FAFAFA;}");

    this->setWindowIcon(QIcon(":/image/graph.png"));

    //mainscene
    QLabel* title = new QLabel(this);
    scene = new MyMainScene(this);
    ui->mainLayout->addWidget(title);
    title->setText("Graph Structure");
    title->setFont(QFont("Comic Sans MS",24,QFont::Bold));
    title->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    title->setMaximumHeight(50);
    //ui->mainLayout->addSpacing(10);
    ui->mainLayout->addWidget(scene);
    ui->mainLayout->setAlignment(Qt::AlignTop);
    scene->setStyleSheet("QGraphicsView{background-color:#FFFFFF;border:1px solid #2c2c2c;border-radius:20px;}");
    scene->show();
    ui->mainLayout->setSpacing(10);

    //TabWidget
    ui->TabWidget->setTabShape(QTabWidget::Triangular);
    ui->TabWidget->setCurrentIndex(0);

    //Graph tab
    ui->TabWidget->setTabText(0,"Graph");
    QLabel* Graph_titleLabel = new QLabel(ui->Graph);
    ui->GraphLayout->addWidget(Graph_titleLabel);
    Graph_titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Graph_titleLabel->setText("Graph Information");
    Graph_titleLabel->setFont(QFont("微软雅黑",20,QFont::Bold));
    Graph_titleLabel->setFixedHeight(110);

    VexNumLabel = new QLabel(ui->Graph);
    ui->GraphLayout->addWidget(VexNumLabel);
    VexNumLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    QString VexNumstr = "Vertex Number: " + QString::number(scene->g->numVex);
    VexNumLabel->setText(VexNumstr);
    VexNumLabel->setFont(QFont("Comic Sans MS",16));
    VexNumLabel->setFixedHeight(35);

    EdgeNumLabel = new QLabel(ui->Graph);
    ui->GraphLayout->addWidget(EdgeNumLabel);
    EdgeNumLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QString EdgeNumstr = "Edge Number: " + QString::number(scene->g->numEdge);
    EdgeNumLabel->setText(EdgeNumstr);
    EdgeNumLabel->setFont(QFont("Comic Sans MS",16));

    connect(scene,&MyMainScene::VexNumChanged,[=](){
        updateVexNum();
    });
    connect(scene,&MyMainScene::EdgeNumChanged,[=](){
        updateEdgeNum();
    });

    //functional tab
    ui->TabWidget->setTabText(1,"Function");
    //DFS
    QMenu *DFSmenu = new QMenu;
    DFSmenu->setFixedWidth(ui->DFSBtn->width());
    DFSmenu->setStyleSheet("QMenu {background:#FFFFFF;border-radius:5px;border:1px solid #2c2c2cE0;}"
                           "QMenu::item {text-align:center;font-family: Corbel; font:bold 18px;border-radius:5px;"
                                "border:1px solid #2c2c2cE0;padding:16px 16px;margin:2px 2px;}"
                           "QMenu::item:selected {background-color:#1a000000;border-radius:5px;}"
                           "QMenu::item:pressed {background-color:#2a000000;border-radius:5px;");

//    Example
//    DFSmenu->setStyleSheet("QMenu {background-color:rgb(89,87,87);border: 3px solid rgb(235,110,36);}"
//    "QMenu::item {font-style: Corbel;font-size: 10px;color: rgb(225,225,225);border: 3px solid rgb(60,60,60);background-color:rgb(89,87,87);\
//                  padding:16px 16px;margin:2px 2px;}"
//    "QMenu::item:selected {background-color:rgb(235,110,36);}"
//    "QMenu::item:pressed {border: 1px solid rgb(60,60,61);background-color: rgb(220,80,6);}");
    QAction* AML_DFSClicked = new QAction("AMLDFS",this);
    QAction* AL_DFSClicked = new QAction("ALDFS",this);
    QAction* Stack_DFSClicked = new QAction("StackDFS",this);
    DFSmenu->addAction(AML_DFSClicked);
    DFSmenu->addAction(AL_DFSClicked);
    DFSmenu->addAction(Stack_DFSClicked);
    ui->DFSBtn->setMenu(DFSmenu);
    connect(AML_DFSClicked,&QAction::triggered,scene,&MyMainScene::AMLDFSClicked);
    connect(AL_DFSClicked,&QAction::triggered,scene,&MyMainScene::ALDFSClicked);
    connect(Stack_DFSClicked,&QAction::triggered,scene,&MyMainScene::StackDFSClicked);

    //BFS
    QMenu *BFSmenu = new QMenu;
    BFSmenu->setFixedWidth(ui->BFSBtn->width());
    BFSmenu->setStyleSheet("QMenu {background:#FFFFFF;border-radius:5px;border:1px solid #2c2c2cE0;}"
                           "QMenu::item {text-align:center;font-family: Corbel; font:bold 18px;border-radius:5px;"
                                "border:1px solid #2c2c2cE0;padding:16px 16px;margin:2px 2px;}"
                           "QMenu::item:selected {background-color:#1a000000;border-radius:5px;}"
                           "QMenu::item:pressed {background-color:#2a000000;border-radius:5px;");
    QAction* AML_BFSClicked = new QAction("AMLBFS",this);
    QAction* AL_BFSClicked = new QAction("ALBFS",this);
    BFSmenu->addAction(AML_BFSClicked);
    BFSmenu->addAction(AL_BFSClicked);
    ui->BFSBtn->setMenu(BFSmenu);
    connect(AML_BFSClicked,&QAction::triggered,scene,&MyMainScene::AMLBFSClicked);
    connect(AL_BFSClicked,&QAction::triggered,scene,&MyMainScene::ALBFSClicked);

    //Prim
    connect(ui->PrimBtn,&QPushButton::clicked,scene,&MyMainScene::PrimClicked);

    //Dijkstra
    connect(ui->DijkstraBtn,&QPushButton::clicked,scene,&MyMainScene::DijkstraClicked);

    //Data structure tab
    ui->TabWidget->setTabText(2,"Data structure");
    ui->TabWidget->setTabVisible(2,false);

    //Item info tab

    //Vex info
    ui->TabWidget->setTabText(3,"Vertex");
    ui->TabWidget->setTabVisible(3,false);
    QLabel* Vertex_titleLabel = new QLabel(ui->VertexInfo);
    ui->infoLayout->addWidget(Vertex_titleLabel);
    Vertex_titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    Vertex_titleLabel->setText("Vertex Information");
    Vertex_titleLabel->setFont(QFont("微软雅黑",20,QFont::Bold));
    Vertex_titleLabel->setFixedHeight(73);
    Id = new QLabel(ui->VertexInfo);
    ui->infoLayout->addWidget(Id);
    Id->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    Id->setFont(QFont("Comic Sans MS",15));
    Id->setFixedHeight(50);
    QWidget* NameWidget = new QWidget(ui->VertexInfo);
    ui->infoLayout->addWidget(NameWidget);
    QHBoxLayout* namelayout = new QHBoxLayout(NameWidget);
    namelabel = new QLabel(NameWidget);
    namelabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    namelabel->setFont(QFont("Comic Sans MS",15));
    namelayout->addWidget(namelabel);
    Name = new QLineEdit(NameWidget);
    Name->setAlignment(Qt::AlignLeft);
    Name->setFont(QFont("Comic Sans MS",15));    
    Name->setReadOnly(true);
    Name->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    namelayout->addWidget(Name);
    mdf_btn = new QPushButton(NameWidget);
    namelayout->addWidget(mdf_btn);
    namelayout->setStretch(0,6);
    namelayout->setStretch(1,4);
    mdf_btn->setIcon(QIcon(":/image/modify.png"));
    mdf_btn->setStyleSheet("QPushButton{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #FAFAFA;}"
                           "QPushButton:hover{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #BDBDBD;}"
                           "QPushButton:pressed{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #E0E0E0;}");
    connect(mdf_btn,&QPushButton::clicked,[=](){
        Name->setReadOnly(false);
        mdf_btn->setIcon(QIcon(":/image/correct.png"));
        mdf_btn->setStyleSheet("QPushButton{background: transparent;border:0px;}");
        NameIsUsed = false;
        LastName = scene->view->Selectedvex->data;
        scene->g->VertexNameSet.remove(Name->text());
    });
    EdgeNumber = new QLabel(ui->VertexInfo);
    ui->infoLayout->addWidget(EdgeNumber);
    EdgeNumber->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    EdgeNumber->setFont(QFont("Comic Sans MS",15));
    ui->infoLayout->setAlignment(Qt::AlignHCenter);

    connect(Name,&QLineEdit::textEdited,[=](){
        QString cur = Name->text();
        bool flag = false;
        for(auto it = scene->g->VertexNameSet.begin(); it != scene->g->VertexNameSet.end(); it++){
            if(*it == cur){
                flag = true;
                break;
            }
        }
        NameIsUsed = flag;
        if(NameIsUsed){
            mdf_btn->setIcon(QIcon(":/image/false.png"));
        }else{
            mdf_btn->setIcon(QIcon(":/image/correct.png"));
        }
    });

    //Edge Info
    ui->TabWidget->setTabText(4,"Edge");
    ui->TabWidget->setTabVisible(4,false);
    QLabel* Edge_titleLabel = new QLabel(ui->EdgeInfo);
    ui->EdgeInfoLayout->addWidget(Edge_titleLabel);
    Edge_titleLabel->setFixedHeight(70);
    Edge_titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    Edge_titleLabel->setText("Edge Information");
    Edge_titleLabel->setFont(QFont("微软雅黑",20,QFont::Bold));
    firstVex = new QLabel(ui->VertexInfo);
    ui->EdgeInfoLayout->addWidget(firstVex);
    firstVex->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    firstVex->setFont(QFont("Comic Sans MS",15));
    firstVex->setFixedHeight(50);
    secondVex = new QLabel(ui->VertexInfo);
    ui->EdgeInfoLayout->addWidget(secondVex);
    secondVex->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    secondVex->setFont(QFont("Comic Sans MS",15));
    secondVex->setFixedHeight(50);

    QWidget* WeightWidget = new QWidget(ui->EdgeInfo);
    ui->EdgeInfoLayout->addWidget(WeightWidget);
    QHBoxLayout* weightlayout = new QHBoxLayout(WeightWidget);
    weightlabel = new QLabel(WeightWidget);
    weightlabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    weightlabel->setFont(QFont("Comic Sans MS",15));
    weightlayout->addWidget(weightlabel);
    Weight = new QLineEdit(ui->VertexInfo);
    ui->EdgeInfoLayout->addWidget(Weight);
    Weight->setAlignment(Qt::AlignLeft);
    Weight->setFont(QFont("Comic Sans MS",15));
    Weight->setReadOnly(true);
    Weight->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    Weight->setValidator(new QRegularExpressionValidator(QRegularExpression("^[1-9]\\d*$")));
    weightlayout->addWidget(Weight);
    mdf_btn2 = new QPushButton(WeightWidget);
    weightlayout->addWidget(mdf_btn2);
    mdf_btn2->setIcon(QIcon(":/image/modify.png"));
    mdf_btn2->setStyleSheet("QPushButton{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #FAFAFA;}"
                           "QPushButton:hover{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #BDBDBD;}"
                           "QPushButton:pressed{border-radius: 2px;border-style:outset;"
                           "border:1px solid #2c2c2cE0;background-color: #E0E0E0;}");
    connect(mdf_btn2,&QPushButton::clicked,[=](){
        Weight->setReadOnly(false);
        mdf_btn2->setIcon(QIcon(":/image/modifying.png"));
    });
    weightlayout->setStretch(0,6);
    weightlayout->setStretch(1,4);
    ui->EdgeInfoLayout->setAlignment(Qt::AlignHCenter);

    connect(this->scene->view,SIGNAL(PresentInfo(QGraphicsItem*)),this,SLOT(AddInfoTab(QGraphicsItem*)));
    connect(this->scene->view,SIGNAL(CloseVertexInfo()),this,SLOT(CloseVertexInfoTab()));
    connect(this->scene->view,SIGNAL(CloseEdgeInfo()),this,SLOT(CloseEdgeInfoTab()));

    //Output TextBroser
    ui->OutputWidget->setFontPointSize(15);
    ui->OutputWidget->setFont(QFont("Comic Sans MS",15));
    connect(this->scene->view,&MyGraphicsView::VertexAdded,[=](){
        QDateTime currentTime=QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Vertex \"" + scene->g->AdjmultiList.back()->data  + "\" was added.");
    });
    connect(this->scene->view,SIGNAL(LineAdded(MyLine*)),this,SLOT(AddEdgeOutput(MyLine*)));
    connect(this->scene->view,SIGNAL(DeleteVertex(MyVertex*)),this,SLOT(DeleteVexOutput(MyVertex*)));
    connect(this->scene->view,SIGNAL(DeleteLine(MyLine*)),this,SLOT(DeleteEdgeOutput(MyLine*)));
    connect(this->scene,SIGNAL(VertexVisited(MyVertex*)),this,SLOT(VisitVexOutput(MyVertex*)));
    connect(this->scene,SIGNAL(EdgeVisited(MyLine*)),this,SLOT(VisitEdgeOutput(MyLine*)));
    connect(this->scene,&MyMainScene::AMLDFSStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  AMLDFS Started.  ---");
    });
    connect(this->scene,&MyMainScene::AMLDFSEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  AMLDFS Ended.  ---");
    });
    connect(this->scene,&MyMainScene::ALDFSStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  ALDFS Started.  ---");
    });
    connect(this->scene,&MyMainScene::ALDFSEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  ALDFS Ended.  ---");
    });
    connect(this->scene,&MyMainScene::StackDFSStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  StackDFS Started.  ---");
    });
    connect(this->scene,&MyMainScene::StackDFSEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  StackDFS Ended.  ---");
    });
    connect(this->scene,&MyMainScene::AMLBFSStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  AMLBFS Started.  ---");
    });
    connect(this->scene,&MyMainScene::AMLBFSEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  AMLBFS Ended.  ---");
    });
    connect(this->scene,&MyMainScene::ALBFSStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  ALBFS Started.  ---");
    });
    connect(this->scene,&MyMainScene::ALBFSEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  ALBFS Ended.  ---");
    });
    connect(this->scene,&MyMainScene::PrimStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Prim Started.  ---");
    });
    connect(this->scene,&MyMainScene::PrimEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Prim Ended.  ---");
    });
    connect(this->scene,&MyMainScene::DijkstraStart,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Dijkstra Started.  ---");
    });
    connect(this->scene,&MyMainScene::DijkstraEnd,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Dijkstra Ended.  ---");
    });
    connect(this->scene,SIGNAL(OutputMinPath(QVector<int>)),this,SLOT(OutputMinPath(QVector<int>)));
    connect(this->scene,&MyMainScene::ProgressInterrupt,[=](){
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "The progress was interrupted.");
    });

    //menu
    QPushButton* menubtn = new QPushButton(this);
    menubtn->setIcon(QIcon(":/image/menu.png"));
    menubtn->resize(50,50);
    menubtn->move(0,0);
    menubtn->setStyleSheet("QPushButton{background:transparent;border:0px;border-radius:5px;}"
                           "QPushButton:hover{background-color:#BDBDBD;border:0px;border-radius:5px;}"
                           "QPushButton:pressed{background-color:#E0E0E0;border:0px;border-radius:5px;}");

    QScrollArea* menuWidget = new QScrollArea(this);
    menuWidget->resize(300,this->height());
    menuWidget->move(-300,0);
    menuWidget->setStyleSheet("QScrollArea{background-color:rgb(215,215,246);border-top-right-radius:20px;border-bottom-right-radius:20px;}");


    connect(menubtn,&QPushButton::clicked,[=](){
        menubtn->setVisible(false);
        QPropertyAnimation* animation = new QPropertyAnimation(menuWidget,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(menuWidget->x(),menuWidget->y(),menuWidget->width(),menuWidget->height()));
        animation->setEndValue(QRect(menuWidget->x()+menuWidget->width(),menuWidget->y(),menuWidget->width(),menuWidget->height()));
        animation->setEasingCurve(QEasingCurve::OutExpo);
        animation->start();
    });

    //Load and Save

    //Load
    QPushButton* loadbtn = new QPushButton(menuWidget);
    loadbtn->setText("Load");
    loadbtn->setIcon(QIcon(":/image/load.png"));
    loadbtn->setIconSize(QSize(30,30));
    loadbtn->setFont(QFont("Comic Sans MS",14));
    loadbtn->setStyleSheet("QPushButton{background:transparent;}"
                           "QPushButton:hover{background-color: #2c2c2cE0;}"
                           "QPushButton:pressed{background-color: #1c2c2cE0;}");
    connect(loadbtn,&QPushButton::clicked,[=](){
        QString path = QFileDialog::getOpenFileName(this,"Load","E:\\QT project\\Graph");
        QFile file(path);
        if(!file.open(QIODeviceBase::ReadOnly)){
            qDebug() << "failed to open file!";
        }else{
            QTextStream stream(&file);
            int numvex;
            stream >> numvex;
            stream.readLine();
            for(int i = 0; i < numvex; i++){
                QString name = stream.readLine();
                qreal x,y;
                stream >> x >> y;
                stream.readLine();
                scene->view->AddVex(QPointF(x,y),13,name);
            }
            int numedge;
            stream >> numedge;
            stream.readLine();
            for(int i = 0; i < numedge; i++){
                int id1,id2;
                stream >> id1 >> id2;
                stream.readLine();
                int w;
                stream >> w;
                stream.readLine();
                scene->view->AddLine(scene->view->AllVertex[id1],scene->view->AllVertex[id2],w);
            }
            file.close();
            QDateTime currentTime = QDateTime::currentDateTime();
            ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Load from a file successfully.");
        }
    });

    //Save
    QPushButton* savebtn = new QPushButton(menuWidget);
    savebtn->setText("Save");
    savebtn->setIcon(QIcon(":/image/save.png"));
    savebtn->setIconSize(QSize(25,25));
    savebtn->setFont(QFont("Comic Sans MS",14));
    savebtn->setStyleSheet("QPushButton{background:transparent;}"
                           "QPushButton:hover{background-color: #2c2c2cE0;}"
                           "QPushButton:pressed{background-color: #1c2c2cE0;}");
    connect(savebtn,&QPushButton::clicked,[=](){
        QString path = QFileDialog::getSaveFileName(this,tr("Save"),"E:\\QT Project\\Graph",tr("save(*.txt)"));
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "failed to open file!";
        }else{
            QTextStream stream(&file);
            stream << scene->g->numVex << "\n";
            for(int i = 0; i < scene->view->AllVertex.size(); i++){
                stream << scene->view->AllVertex[i]->data << "\n";
                stream << scene->view->AllVertex[i]->center.x() << " " << scene->view->AllVertex[i]->center.y() << "\n";
            }
            stream << scene->g->numEdge << "\n";
            for(int i = 0; i < scene->view->AllLine.size(); i++){
                stream << scene->view->AllLine[i]->id_i << " " << scene->view->AllLine[i]->id_j << "\n";
                stream << scene->view->AllLine[i]->weight << "\n";
            }
            stream.flush();
            file.close();
            QDateTime currentTime = QDateTime::currentDateTime();
            ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Save to a file successfully.");
        }
    });

    QVBoxLayout* menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignTop);
    menuLayout->setContentsMargins(0,50,0,10);
    QLabel* menutitle = new QLabel(menuWidget);
    menutitle->setMaximumHeight(150);
    menutitle->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    menutitle->setText("   Settings");
    menutitle->setFont(QFont("Comic Sans MS",28));
    menuLayout->addWidget(menutitle);
    menuLayout->setSpacing(30);
    QLabel* label1 = new QLabel(menuWidget);
    label1->setText("    Load a graph from a file");
    label1->setFont(QFont("Comic Sans MS",12,-1,true));
    label1->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    menuLayout->addSpacerItem(new QSpacerItem(300,60));
    menuLayout->addWidget(label1);
    menuLayout->addWidget(loadbtn);
    loadbtn->setMinimumHeight(60);
    QLabel* label2 = new QLabel(menuWidget);
    label2->setText("    Save a graph to a file");
    label2->setFont(QFont("Comic Sans MS",12,-1,true));
    label2->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    menuLayout->addSpacerItem(new QSpacerItem(300,50));
    menuLayout->addWidget(label2);
    menuLayout->addWidget(savebtn);
    savebtn->setMinimumHeight(60);

    menuLayout->addSpacerItem(new QSpacerItem(300,50));
    QLabel* label3 = new QLabel(menuWidget);
    label3->setText("    Clear all Output lists");
    label3->setFont(QFont("Comic Sans MS",12,-1,true));
    label3->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    menuLayout->addWidget(label3);
    QPushButton* clearOutputbtn = new QPushButton(menuWidget);
    clearOutputbtn->setText("Clear");
    clearOutputbtn->setFont(QFont("Comic Sans MS",14));
    clearOutputbtn->setStyleSheet("QPushButton{background:transparent;}"
                           "QPushButton:hover{background-color: #2c2c2cE0;}"
                           "QPushButton:pressed{background-color: #1c2c2cE0;}");
    clearOutputbtn->setMinimumHeight(60);
    clearOutputbtn->setIcon(QIcon(":/image/clear1.png"));
    clearOutputbtn->setIconSize(QSize(25,25));
    menuLayout->addWidget(clearOutputbtn);
    connect(clearOutputbtn,&QPushButton::clicked,[=](){
        ui->OutputWidget->clear();

    });



    //back from menu
    menuLayout->addSpacerItem(new QSpacerItem(300,50));
    QPushButton* backbtn = new QPushButton(menuWidget);
    backbtn->setText("Back");
    backbtn->setIcon(QIcon(":/image/back.png"));
    backbtn->setIconSize(QSize(20,20));
    backbtn->setFont(QFont("Comic Sans MS",14));
    backbtn->setStyleSheet("QPushButton{background:transparent;border-bottom-right-radius:20px;}"
                           "QPushButton:hover{background-color: #2c2c2cE0;}"
                           "QPushButton:pressed{background-color: #1c2c2cE0;}");
    backbtn->setMinimumHeight(60);
    menuLayout->addWidget(backbtn);
    connect(backbtn,&QPushButton::clicked,[=](){
        QPropertyAnimation* animation = new QPropertyAnimation(menuWidget,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(menuWidget->x(),menuWidget->y(),menuWidget->width(),menuWidget->height()));
        animation->setEndValue(QRect(menuWidget->x()-menuWidget->width(),menuWidget->y(),menuWidget->width(),menuWidget->height()));
        animation->setEasingCurve(QEasingCurve::OutExpo);
        animation->start();
        menubtn->setVisible(true);
    });

}

void MainWindow::AddInfoTab(QGraphicsItem* item){  
    if(item->type() == MyVertex::Type){
        ui->TabWidget->setTabVisible(3,true);
        ui->TabWidget->setCurrentIndex(3);
        MyVertex* vex = qgraphicsitem_cast<MyVertex*>(item);
        int id = vex->id;
        int EdgeNum = 0;
        ALEdge* p = scene->g->AdjList[id]->firstedge;
        while(p){
            EdgeNum++;
            p = p->next;
        }
        Id->setText("Id:  " + QString::number(id));
        namelabel->setText("Name:  ");
        Name->setText(vex->data);
        EdgeNumber->setText("Edge Number: " + QString::number(EdgeNum));
    }else if(item->type() == MyLine::Type){
        MyLine* line = qgraphicsitem_cast<MyLine*>(item);
        ui->TabWidget->setTabVisible(4,true);
        ui->TabWidget->setCurrentIndex(4);
        int firstvex = line->id_i;
        QString firstname = line->vex_i->data;
        int secondvex = line->id_j;
        QString secondname = line->vex_j->data;
        firstVex->setText("First Vertex:  " + QString::number(firstvex) + "     " + firstname);
        secondVex->setText("Second Vertex:  " + QString::number(secondvex) + "     " + secondname);
        weightlabel->setText("Edge Weight: ");
        Weight->setText(QString::number(line->weight));
    }
}

void MainWindow::CloseVertexInfoTab(){
    ui->TabWidget->setTabVisible(3,false);
    ui->TabWidget->setCurrentIndex(0);
}

void MainWindow::CloseEdgeInfoTab(){
    ui->TabWidget->setTabVisible(4,false);
    ui->TabWidget->setCurrentIndex(0);
}

void MainWindow::updateVexNum(){
    QString VexNumstr = "Vertex number: " + QString::number(scene->g->numVex);
    VexNumLabel->setText(VexNumstr);
}

void MainWindow::updateEdgeNum(){
    QString EdgeNumstr = "Edge number: " + QString::number(scene->g->numEdge);
    EdgeNumLabel->setText(EdgeNumstr);
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    switch (event->key()) {
        case Qt::Key_Return:
            if(Name->isReadOnly() == false){
                QString changedname = Name->text();
                if(NameIsUsed == false){
                    Name->setReadOnly(true);
                    scene->view->Selectedvex->data = changedname;
                    scene->view->Selectedvex->Vex->data = changedname;
                    scene->view->Selectedvex->ALVex->data = changedname;
                    mdf_btn->setIcon(QIcon(":/image/modify.png"));
                    mdf_btn->setStyleSheet("QPushButton{border-radius: 2px;border-style:outset;"
                                           "border:1px solid #2c2c2cE0;background-color: #FAFAFA;}"
                                           "QPushButton:hover{border-radius: 2px;border-style:outset;"
                                           "border:1px solid #2c2c2cE0;background-color: #BDBDBD;}"
                                           "QPushButton:pressed{border-radius: 2px;border-style:outset;"
                                           "border:1px solid #2c2c2cE0;background-color: #E0E0E0;}");
                    scene->g->VertexNameSet.insert(changedname);
                    VertexRename(scene->view->Selectedvex);
                }
            }
            if(Weight->isReadOnly() == false){
                Weight->setReadOnly(true);
                int changedweight = Weight->text().toUInt();
                mdf_btn2->setIcon(QIcon(":/image/modify.png"));
                scene->view->Selectedline->weight = changedweight;
                scene->view->Selectedline->ALedge_i->weight = changedweight;
                scene->view->Selectedline->ALedge_j->weight = changedweight;
                scene->view->Selectedline->edge->weight = changedweight;
                EdgeChangeWeight(scene->view->Selectedline);
            }
            break;
    }
}

void MainWindow::AddEdgeOutput(MyLine* line){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Edge between \""
         + scene->g->AdjmultiList[line->id_i]->data + "\" and \"" + scene->g->AdjmultiList[line->id_j]->data
            + "\" was added.");
}

void MainWindow::DeleteVexOutput(MyVertex* vex){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Vertex \""
        + vex->data + "\" was removed.");
}

void MainWindow::DeleteEdgeOutput(MyLine* line){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Edge between \""
         + scene->g->AdjmultiList[line->id_i]->data + "\" and \"" + scene->g->AdjmultiList[line->id_j]->data
            + "\" was removed.");
}

void MainWindow::VertexRename(MyVertex* vex){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Vertex \""
        + LastName + "\" has been renamed \"" + vex->data + "\".");
}

void MainWindow::EdgeChangeWeight(MyLine* line){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "The weight of the edge between \""
         + scene->g->AdjmultiList[line->id_i]->data + "\" and \"" + scene->g->AdjmultiList[line->id_j]->data
            + "\" has been changed to " + QString::number(line->weight) + ".");
}

void MainWindow::VisitVexOutput(MyVertex* vex){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "Vertex \""
        + vex->data + "\" was visited.");
}

void MainWindow::VisitEdgeOutput(MyLine* line){
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "The edge between \""
         + scene->g->AdjmultiList[line->id_i]->data + "\" and \"" + scene->g->AdjmultiList[line->id_j]->data
            + "\" was visited.");
}

void MainWindow::OutputMinPath(QVector<int> v){
    int idx = 0;
    for(int i = 0; i < v.size(); i++){
        if(v[i] == 0){
            idx = i;
            break;
        }
    }
    ui->OutputWidget->append("Shortest path from vertex \"" + scene->g->AdjmultiList[idx]->data + "\"" + " to others:");
    for(int i = 0; i < v.size(); i++){
        if(i == idx){
            continue;
        }
        if(v[i] != INT_MAX){
            ui->OutputWidget->append(scene->g->AdjmultiList[idx]->data + " to " + scene->g->AdjmultiList[i]->data + ":   " + QString::number(v[i]));
        }else{
            ui->OutputWidget->append(scene->g->AdjmultiList[idx]->data + " to " + scene->g->AdjmultiList[i]->data + ":   Inf");
        }
    }
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->OutputWidget->append("[" + currentTime.toString("hh:mm:ss") + "] " + "---  Finished.  ---");
}

MainWindow::~MainWindow()
{
    delete ui;
}

