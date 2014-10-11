#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T13:02:11
#
#-------------------------------------------------

QT       += sql core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pipewrap
TEMPLATE = app
CONFIG += c++11
LIBS += -L/usr/lib/fglrx -lGL
#QTPLUGIN += qsqlmysql
#QTPLUGIN += qsqlpsql

SOURCES += main.cpp\
        workflowtreeview.cpp \
    tooloptionsdialog.cpp \
    fromtodialog.cpp \
    mainwindow.cpp \
    databasedialog.cpp \
    pipestatus.cpp \
    assistant.cpp

HEADERS  += workflowtreeview.h \
    main.h \
    tooloptionsdialog.h \
    fromtodialog.h \
    mainwindow.h \
    databasedialog.h \
    pipestatus.h \
    assistant.h

LIBS += -L"/opt/mysql/server-5.6/lib"
#LIBS += -L"/usr/local/pgsql/lib"
#LIBS += -L"/usr/lib/postgresql/9.3/lib"
INCLUDEPATH += "/opt/mysql/server-5.6/include"
#INCLUDEPATH += "/usr/include/postgresql"
#INCLUDEPATH += "/usr/local/pgsql/include"
#unix:LIBS += -L/usr/lib/x86_64-linux-gnu/mesa/

RESOURCES += \
#   WorkflowTreeView.qrc \
    Pipewrap.qrc

OTHER_FILES += \
    Documents/pipewrap.qch \
    Documents/pipewrap.qhc \
    Documents/about.txt \
    Documents/pipewrap.qhcp \
    Documents/pipewrap.qhp \
    Documents/html/index.html \
    Documents/html/openpipe.html \
    Documents/html/varientlister.html \
    Documents/html/contextMenu.html \
    Documents/html/mainMenu.html \
    Documents/html/SNPgenerator.html \
    Documents/html/toolbar.html \
    Documents/html/variantLister.html \
    Documents/html/analyseBax.html \
    Documents/html/plotroc.html \
    Documents/html/simbas.html

