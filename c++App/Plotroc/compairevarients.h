#ifndef COMPAIREVARIENTS_H
#define COMPAIREVARIENTS_H
#include <QStringList>
#include <QVector>
#include <QString>
#include <QCommandLineParser>
#include "rocdatabase.h"

class CompaireVarients : public RocDataBase
{
public:
    CompaireVarients(const QStringList& illuminaFiles
                     ,const QStringList pacBioFiles
                     ,int lowerBound
                     ,int upperBoud);
    //CompaireVarients(QCommandLineParser& clp);
    virtual ~CompaireVarients();
    virtual void operator() ();
    virtual QVector<  double> getTprs(int pos) const;
    virtual QVector<  double> getFprs(int pos) const;
    virtual QList<QString> getCurveNames() const;
    virtual QList<double> getFrequencies() const;
    virtual QString getTitle() const;
    virtual QString getPdfFilename() const;
private:
    void init();
    void addRocCurve(long double threshold);
    void genRocValues(QVector<bool> isPositives, QString title);
    void ReadFiles();

    struct LocusData
    {
        char base;
        int coverage;
        int pSNPcount;
        long double pvalue;
    };
    QList<QVector<LocusData>> illuminaData;
    QList<QVector<LocusData>> pacBioData;
    QStringList titles;
    QStringList illuminaFiles;
    QStringList pacBioFiles;
    int lowerIndex;
    int upperIndex;
    const QList<long double> probThresholds =
    {
        1.0E-5
        ,1.0E-10
        ,1.0E-20
        ,1.0E-50
        ,1.0E-100
    };

    QVector<LocusData> readLocusData(const QString& filename);

    QList<QVector< double>> rocTprs;
    QList<QVector< double>> rocFprs;
};

#endif // COMPAIREVARIENTS_H
