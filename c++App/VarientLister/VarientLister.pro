#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T03:08:20
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = VarientLister
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

#-i stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
TEMPLATE = app

LIBS += -L$$PWD/../bamtools/lib/ -lbamtools
INCLUDEPATH += $$PWD/../bamtools/include

SOURCES += main.cpp \
    varientcaller.cpp

HEADERS += \
    varientcaller.h \
    main.h
