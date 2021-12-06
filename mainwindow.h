#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygraphicsview.h"
#include "graph.h"
#include "edge.h"
#include "vertex.h"
#include "myline.h"
#include "myvertex.h"
#include "mymainscene.h"
#include "aledge.h"
#include "alvertex.h"
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QPushButton>
#include <QMenu>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QPropertyAnimation>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  

private:
    MyMainScene* scene = nullptr;

    //Graph Tab
    QLabel* VexNumLabel;
    QLabel* EdgeNumLabel;

    //Vertex Tab
    QLineEdit* Name;
    QLabel* namelabel;
    QLabel* Id;
    QLabel* EdgeNumber;
    bool NameIsUsed = false;
    QPushButton* mdf_btn;
    QPushButton* mdf_btn2;
    QString LastName;

    //Edge Tab
    QLineEdit* Weight;
    QLabel* weightlabel;
    QLabel* firstVex;
    QLabel* secondVex;

    void keyPressEvent(QKeyEvent* event);

    void VertexRename(MyVertex* vex);
    void EdgeChangeWeight(MyLine* line);

    Ui::MainWindow *ui;

private slots:
    void AddInfoTab(QGraphicsItem* item);
    void CloseVertexInfoTab();
    void CloseEdgeInfoTab();
    void updateVexNum();
    void updateEdgeNum();
    void AddEdgeOutput(MyLine* line);
    void DeleteVexOutput(MyVertex* vex);
    void DeleteEdgeOutput(MyLine* line);
    void VisitVexOutput(MyVertex* vex);
    void VisitEdgeOutput(MyLine* line);
    void OutputMinPath(QVector<int> v);
};
#endif // MAINWINDOW_H
