#ifndef ROCDATA_H
#define ROCDATA_H
#include <QString>
#include <QVector>
#include <QCommandLineParser>
//#include "frequencypartition.h"


class RocData
{
public:
    RocData(QCommandLineParser& clp);

    QVector<  double> getTprs(int pos) const;
    QVector<  double> getFprs(int pos) const;
    QList<QString> getPVFilenames() const;



    QVector<bool> getIsTruePositives() const;
    QList<QVector<  double> > getPVVectors() const;
    QVector<  double> getPvVector(int pos) const;

private:
    void init();
    void readData();
    void readPsnpDetails(int maxFileLenght);
    void genRocValues();

    QList<QString> pvFilenames;
    QVector<bool> isPositives;
    QList<QVector< double>> pvVectors;
    QList<QVector< double>> rocTprs;
    QList<QVector< double>> rocFprs;

    QCommandLineParser& clp;
    //FrequencyPartition freqPartition;
};

#endif // ROCDATA_H
