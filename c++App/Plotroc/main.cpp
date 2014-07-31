#include <QCoreApplication>
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QVariant>
#include "plotwindow.h"
#include "main.h"
#include "compairevarients.h"

//-I lociLIB_001.csv -P lociLIB_002.csv -l 250 -u 7200
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);
    clp.process(a);
    if (clp.value(dataIllumina1).size()>0)
    {
        compareIlluminaPacBioReads(clp);
    }
    if (clp.value(inpSNPs).size()> 0)
    {
        plotSimRoc(clp);
    }


    return 0;//a.exec();
}

void plotSimRoc(QCommandLineParser& clp)
{
    RocData rocData(clp);
    PlotWindow pw(&rocData);
    pw.show();
}

void compareIlluminaPacBioReads(QCommandLineParser& clp)
{
    QStringList illuminaData;
    if (clp.value(dataIllumina1).size()> 0)
        illuminaData << clp.value(dataIllumina1);
    if (clp.value(dataIllumina1).size()> 0)
        illuminaData << clp.value(dataIllumina2);
    if (clp.value(dataIllumina1).size()> 0)
        illuminaData << clp.value(dataIllumina3);

    QStringList pacBioData;
    if (clp.value(dataPacBio1).size()> 0) pacBioData << clp.value(dataPacBio1);
    if (clp.value(dataPacBio2).size()> 0) pacBioData << clp.value(dataPacBio2);
    if (clp.value(dataPacBio3).size()> 0) pacBioData << clp.value(dataPacBio3);
    if (clp.value(dataPacBio4).size()> 0) pacBioData << clp.value(dataPacBio4);
    if (clp.value(dataPacBio5).size()> 0) pacBioData << clp.value(dataPacBio5);
    if (clp.value(dataPacBio6).size()> 0) pacBioData << clp.value(dataPacBio6);

    if (illuminaData.size() > 0 && pacBioData.size() > 0)
    {
        CompaireVarients compiareData(illuminaData
                                      , pacBioData
                                      ,clp.value(lowersLocus).toInt()
                                      ,clp.value(upperLocus).toInt());

        //CompaireVarients compiareData(QCommandLineParser& clp));
        compiareData();
        PlotWindow pw(&compiareData);
        pw.show();
    }
}


void configCommandLineParser(QCommandLineParser& clp)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.0.0.1");

    clp.setApplicationDescription("Create ROC curves from list of pvalues");
    clp.addHelpOption();
    clp.addVersionOption();
    //parser.addOption(inPvPath);
    clp.addOption(inpSNPs);
    clp.addOption(dataIllumina1);
    clp.addOption(dataIllumina2);
    clp.addOption(dataIllumina3);
    clp.addOption(dataPacBio1);
    clp.addOption(dataPacBio2);
    clp.addOption(dataPacBio3);
    clp.addOption(dataPacBio4);
    clp.addOption(dataPacBio5);
    clp.addOption(dataPacBio6);
    clp.addOption(lowersLocus);
    clp.addOption(upperLocus);
    clp.addOption(titleRoc);
}
