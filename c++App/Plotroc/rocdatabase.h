#ifndef ROCDATABASE_H
#define ROCDATABASE_H
#include <QVector>
#include <QList>
#include <QString>
#include "rocdatabase.h"

class RocDataBase
{
public:
    RocDataBase();
    virtual void operator() () = 0;
    virtual QVector<  double> getTprs(int pos) const = 0;
    virtual QVector<  double> getFprs(int pos) const = 0;
    virtual QList<QString> getCurveNames() const = 0;
    virtual QList<double> getFrequencies() const = 0;
    virtual QString getTitle() const = 0;
    virtual QString getPdfFilename() const = 0;
private:

};

#endif // ROCDATABASE_H
