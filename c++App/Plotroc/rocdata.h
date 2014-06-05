#ifndef ROCDATA_H
#define ROCDATA_H
#include <QString>
#include <QVector>
#include <QCommandLineParser>


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
    void readTruePositeves(int maxFileLenght);
    void genRocValues();

    QList<QString> pvFilenames;
    QVector<bool> isPositives;
    QList<QVector< double>> pvVectors;
    QList<QVector< double>> rocTprs;
    QList<QVector< double>> rocFprs;

    QCommandLineParser& clp;
};

#endif // ROCDATA_H
