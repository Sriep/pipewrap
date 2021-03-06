#include <QDir>
#include <QTextStream>
#include <algorithm>
#include <QVector>
#include <vector>
#include "rocdata.h"
#include "main.h"
#include <cfloat>
#include <qdebug.h>

/*
RocData::RocData()
{
}*/

RocData::RocData(QCommandLineParser& clp)
    : RocDataBase()
    , pSnpFile(clp.value(inpSNPs))
    , title(clp.value(titleRoc))
{
    init();
}

RocData::~RocData()
{
}



void RocData::init()
{
    readData();
    genRocValues();

}

void RocData::operator()()
{
}

void RocData::readData()
{
    QDir* pvDir = new QDir(QDir::current());
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
    frequencies.clear();
    QFile inFile(pSnpFile);
    if (inFile.open((QFile::ReadOnly)))
    {
        QTextStream inv(&inFile);
        inv.readLine();
        do
        {
            QString row = "";
            int inNum = 0;
            QString csBase = "";
            QString pSnpBase = "";
            double frequency = 0;

            inv >> row >> inNum >> csBase >> pSnpBase >> frequency;
            vareintLoci.append(--inNum); // rebase from 1 to 0
            frequencies.append(frequency);
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
        QVector<double> tprs;
        QVector<double> fprs;
        long double p = 1.0;
        bool hitYAxis = false;
        do
        {
            if (!(p>DBL_MIN))
            {
                p = 0.0;
                hitYAxis = true;
            }
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
            if (!hitYAxis) hitYAxis = 0 == fp;
            tprs.append(tp/(tp+fn));
            fprs.append(fp/(fp+tn));
            p = p < 0.1 ? p/10.0 : p-0.1;
        }
        while ( ! hitYAxis);
        //while (p > DBL_MIN && ! hitYAxis);
        rocTprs.append(tprs);
        rocFprs.append(fprs);
    }
}

QList<QString> RocData::getCurveNames() const
{
    return pvFilenames;
}

QList<double> RocData::getFrequencies() const
{
    return frequencies;
}

QVector< double> RocData::getTprs(int pos) const
{
    return rocTprs[pos];
}


QVector< double> RocData::getFprs(int pos) const
{
    return rocFprs[pos];
}

QString RocData::getTitle() const
{
    return title;
    //return QString("A ROC curve comparing pSNP calling from simulated\n"
   //                "reads with the known position of generated pSNPs\n"
   //                "as recorded by the simulation process.");
}

QString RocData::getPdfFilename() const
{
    return QString("ROC_SimulationResults.pdf");
}




































