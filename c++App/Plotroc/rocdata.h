#ifndef ROCDATA_H
#define ROCDATA_H
#include <QString>
#include <QVector>
#include <QCommandLineParser>
#include "rocdatabase.h"
//#include "frequencypartition.h"


class RocData : public RocDataBase
{
public:
    //RocData();
    RocData(QCommandLineParser& clp);
    virtual ~RocData();
    virtual void operator() ();
    virtual QVector<double> getTprs(int pos) const;
    virtual QVector<double> getFprs(int pos) const;
    virtual QList<QString> getCurveNames() const; //returns pvFilenames
    virtual QList<double> getFrequencies() const;
    virtual QString getTitle() const;
    virtual QString getPdfFilename() const;
private:
    QVector<bool> getIsTruePositives() const;
    QList<QVector< double> > getPVVectors() const;
    QVector<  double> getPvVector(int pos) const;

    void init();
    void readData();
    void readPsnpDetails(int maxFileLenght);
    void genRocValues();

    QList<QString> pvFilenames;
    QVector<bool> isPositives;
    QList<QVector< double>> pvVectors;
    QList<QVector< double>> rocTprs;
    QList<QVector< double>> rocFprs;
    QList<double> frequencies;

    QString pSnpFile;
    //QCommandLineParser& clp;
    //FrequencyPartition freqPartition;
};

#endif // ROCDATA_H
