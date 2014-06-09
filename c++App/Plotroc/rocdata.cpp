#include <QDir>
#include <QTextStream>
#include <algorithm>
#include <QVector>
#include <vector>
#include "rocdata.h"
#include "main.h"
#include <cfloat>
#include <qdebug.h>

RocData::RocData(QCommandLineParser& clp)
    : clp(clp)
{
    init();
}



void RocData::init()
{
    readData();
    genRocValues();

}
QList<QString> RocData::getPVFilenames() const
{
    return pvFilenames;
}

void RocData::readData()
{
    QString pvPath = clp.value(inPvPath);
    //QDir* pvDir = new QDir(pvPath);
    QDir* pvDir = new QDir("/media/sdb2/Projects/Source/Dissertation/c++App/build-Plotroc-Desktop_Qt_5_3_0_GCC_64Bit_GDB7_7_1-Debug/");
    pvFilenames = pvDir->entryList(QStringList() << "*.pvalues.csv",
                                   QDir::Files);

    int maxFileLenght = 0;
    for (int i=0 ; i < pvFilenames.size() ; i++)
    {
        QFile inFile(pvFilenames[i]);
        if (inFile.open((QFile::ReadOnly)))
        {
            QTextStream inpv(&inFile);
            QVector< double> pvalues;
            int fileLenght =0;
            do
            {
                double inDouble;
                inpv.setRealNumberPrecision(64);
                inpv >> inDouble;
                pvalues.append(inDouble);
                fileLenght++;
            } while (!inpv.atEnd());
            pvVectors.append(pvalues);
            if (fileLenght > maxFileLenght) maxFileLenght = fileLenght;
        }
    }
    readPsnpDetails(maxFileLenght);
}

void RocData::readPsnpDetails(int maxFileLenght)
{
    QList<int> vareintLoci;
    //QList<int> frequencies;
    QFile inFile(clp.value(inpSNPs));
    if (inFile.open((QFile::ReadOnly)))
    {
        QTextStream inv(&inFile);
        inv.readLine();
        do
        {
            int inNum;
            char csBase;
            char pSnpBase;
            int frequency;

            inv >> inNum;// >> csBase >> pSnpBase >> frequency;
            vareintLoci.append(--inNum); // rebase from 1 to 0
            //frequencies.append(frequency);
            inv.readLine();
        } while (!inv.atEnd());
    }
    std::sort(vareintLoci.begin(), vareintLoci.end());
    int varient = 0;
    for (int i = 0 ; i < maxFileLenght ; i++)
    {
        if (vareintLoci.length() > varient)
        {
            bool isPsnp = i == vareintLoci[varient];
            isPositives.append(isPsnp);
            if (isPsnp) varient++;
        }
        else
            isPositives.append(false);
    }
}

void RocData::genRocValues()
{
    for (int result = 0 ; result < pvVectors.size() ; result++)
    {
        QVector< double> tprs;
        QVector<double> fprs;
        long double p = 1;
        bool hitYAxis = false;
        do
        {
            long double tp = 0.0;
            long double fp = 0.0;
            long double tn = 0.0;
            long double fn = 0.0;
            //qDebug() << pvVectors[result].size();
            for (int i = 0 ; i < pvVectors[result].size() ; i++)
            {
                bool isVar = isPositives[i];
                //qDebug() << pvVectors[result].at(i);
                bool guess = pvVectors[result].at(i) <= p;
                tp += isVar && guess ? 1 : 0;
                fp += !isVar && guess ? 1 : 0;
                tn += !isVar && !guess ? 1 : 0;
                fn += isVar && !guess ? 1 : 0;
            }
            hitYAxis = 0 == fp;            
            tprs.append(tp/(tp+fn));
            fprs.append(fp/(fp+tn));
            p = p < 0.1 ? p/10.0 : p-0.1;
        }
        while (p > DBL_MIN && ! hitYAxis);
        rocTprs.append(tprs);
        rocFprs.append(fprs);
        int i;
        i++;
    }
}

QVector< double> RocData::getTprs(int pos) const
{
    return rocTprs[pos];
}


QVector< double> RocData::getFprs(int pos) const
{
    return rocFprs[pos];
}

QList<QVector< double> > RocData::getPVVectors() const
{
    return pvVectors;
}
QVector< double> RocData::getPvVector(int pos) const
{
    return pvVectors[pos];
}

QVector<bool> RocData::getIsTruePositives() const
{
    return isPositives;
}


































