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
#QTPLUGIN += qsqlmysql
#QTPLUGIN += qsqlpsql

SOURCES += main.cpp\
        workflowtreeview.cpp \
    tooloptionsdialog.cpp \
    fromtodialog.cpp \
    mainwindow.cpp \
    databasedialog.cpp \
    pipestatus.cpp \

HEADERS  += workflowtreeview.h \
    main.h \
    tooloptionsdialog.h \
    fromtodialog.h \
    mainwindow.h \
    databasedialog.h \
    pipestatus.h \

LIBS += -L"/opt/mysql/server-5.6/lib"
LIBS += -L"/usr/local/pgsql/lib"
INCLUDEPATH += "/opt/mysql/server-5.6/include"
INCLUDEPATH += "/usr/local/pgsql/include"

RESOURCES += \
#   WorkflowTreeView.qrc \
    Pipewrap.qrc

