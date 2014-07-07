#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T23:56:52
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += printsupport
#QT       -= gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plotroc
#CONFIG   += console
#CONFIG   -= app_bundle

TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp \
    plotwindow.cpp \
    qcpdocumentobject.cpp \
    qcustomplot.cpp \
    rocdata.cpp \
    options.cpp

OTHER_FILES += \
    Plotroc.pro.user

HEADERS += \
    main.h \
    plotwindow.h \
    qcpdocumentobject.h \
    qcustomplot.h \
    rocdata.h \
    options.h
