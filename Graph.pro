QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aledge.cpp \
    alvertex.cpp \
    edge.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    myline.cpp \
    mymainscene.cpp \
    myvertex.cpp \
    vertex.cpp

HEADERS += \
    aledge.h \
    alvertex.h \
    edge.h \
    graph.h \
    mainwindow.h \
    mygraphicsview.h \
    myline.h \
    mymainscene.h \
    myvertex.h \
    vertex.h

FORMS += \
    mainwindow.ui

RC_ICONS = Graph.ico

RC_FILE += Icon.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
