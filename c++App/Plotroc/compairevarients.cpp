#include <QFile>
#include <QTextStream>
#include <cfloat>
#include <QList>
#include <QVariant>
#include <QCommandLineParser>
#include "main.h"
#include "compairevarients.h"


CompaireVarients::CompaireVarients(const QStringList &illuminaFiles
                                   , const QStringList pacBioFiles
                                   , int lowerBound
                                   , int upperBoud)
    :    RocDataBase()
    , illuminaFiles(illuminaFiles)
    , pacBioFiles(pacBioFiles)
    , lowerIndex(lowerBound)
    , upperIndex(upperBoud)
{
    ReadFiles();
}
/*
CompaireVarients::CompaireVarients(QCommandLineParser &clp)
{
    init();
    ReadFiles();
}

void CompaireVarients::init()
{

}*/

CompaireVarients::~CompaireVarients()
{
}

void CompaireVarients::operator()()
{
    for ( int i = 0 ; i < probThresholds.size() ; i++ )
    {
        addRocCurve(probThresholds[i]);
    }
}

void CompaireVarients::addRocCurve(long double threshold)
{
    for ( int i = 0 ; i < illuminaData.size() ; i++ )
    {
        QVector<bool> isPositives;
        for ( int locus = 0 ; locus < illuminaData[i].size() ; locus++ )
        {
            isPositives.push_back(illuminaData[i].at(locus).pvalue < threshold);
        }
        QString title = illuminaFiles[i] + " thresold "
                + QVariant((double)threshold).toString();
        genRocValues(isPositives, title);
    }
}

void CompaireVarients::genRocValues(QVector<bool> isPositives, QString title)
{
    for (int result = 0 ; result < pacBioData.size() ; result++)
    //for (int result = 0 ; result < pvVectors.size() ; result++)
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
            //for (int i = 0 ; i < pvVectors[result].size() ; i++)
            for (int i = 0 ; i < pacBioData[result].size() ; i++)
            {
                bool isVar = isPositives[i];
                //bool guess = pvVectors[result].at(i) <= p;
                bool guess = pacBioData[result].at(i).pvalue <= p;
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
        while (! hitYAxis);
        //while (p > DBL_MIN && ! hitYAxis);
        titles.append(title);
        rocTprs.append(tprs);
        rocFprs.append(fprs);
    }
}

void CompaireVarients::ReadFiles()
{
    for ( int i = 0 ; i < illuminaFiles.size() ; i++ )
    {
        if (illuminaFiles[i].size()>0)
            illuminaData << readLocusData(illuminaFiles[i]);
    }

    for ( int i = 0 ; i < pacBioFiles.size() ; i++ )
    {
        if (pacBioFiles[i].size()>0)
            pacBioData << readLocusData(pacBioFiles[i]);
    }
}

QVector<CompaireVarients::LocusData> CompaireVarients::readLocusData(
        const QString &filename)
{
    QVector<CompaireVarients::LocusData> locusData;
    QFile inFile(filename);
    if (inFile.open((QFile::ReadOnly)))
    {
        QTextStream inData(&inFile);
        inData.readLine();
        while(!inData.atEnd())
        {
            int locus;
            char base;
            int coverage;
            int averageQality;
            char pSNP;
            int pSNPcount;
            int pSNPpct;
            double pValue;
            char tab;

            inData >> locus >> tab >> base >> coverage >> averageQality
                   >> tab >> pSNP >> pSNPcount >> pSNPpct;
            inData.setRealNumberPrecision(64);
            inData >> pValue;
            if (lowerIndex < locus && locus < upperIndex)
            {
                LocusData data = { base, coverage, pSNPcount, pValue };
                locusData << data;
            }
        }
    }
    return locusData;
}


QList<QString> CompaireVarients::getCurveNames() const
{
    return titles;
}

QList<double> CompaireVarients::getFrequencies() const
{
    QList<double> l;
    return l;
}

QString CompaireVarients::getTitle() const
{
    return QString("A ROC curve comparing pSNP calling across different\n"
                   "sequencing experiments A ture positive is when both \n"
                   "agree on a pSNP call. False positive or negitive when\n"
                   "there is a disagreement");
}

QString CompaireVarients::getPdfFilename() const
{
    return QString("ROC_CompareResults.pdf");
}

QVector< double> CompaireVarients::getTprs(int pos) const
{
    return rocTprs[pos];
}


QVector< double> CompaireVarients::getFprs(int pos) const
{
    return rocFprs[pos];
}





































