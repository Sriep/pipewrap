#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T15:00:51
#
#-------------------------------------------------

QT       += core gui

#QT       -= gui

TARGET = potroc
#CONFIG += console
CONFIG += c++11
#CONFIG   -= app_bundle
QT       += sql core gui


TEMPLATE = app


SOURCES += main.cpp \
    rocdata.cpp \
    qcustomplot.cpp \
    qcpdocumentobject.cpp \
    plotwindow.cpp

HEADERS += \
    main.h \
    rocdata.h \
    qcustomplot.h \
    qcpdocumentobject.h \
    plotwindow.h
