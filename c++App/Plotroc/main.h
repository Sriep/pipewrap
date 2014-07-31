#ifndef MAIN_H
#define MAIN_H
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <string>
using namespace std;

void configCommandLineParser(QCommandLineParser& parser);
void plotSimRoc(QCommandLineParser& clp);
void compareIlluminaPacBioReads(QCommandLineParser& clp);

static string programName = "plotroc";
static string version = "1.0.0.1";
//static QCommandLineParser clp;
/*const QCommandLineOption inPvPath(
            QStringList() << "i" << "in-sufix",
            "File contianing list of pvalues",
            "filename",
            "");
            */
const QCommandLineOption inpSNPs(
            QStringList() << "s" << "pSNPs",
           "Output file from pSNPgenerator.R",
           "filename",
            "");

const QCommandLineOption dataIllumina1(
            QStringList() << "I" << "I1lumina1",
           "Illumina read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataIllumina2(
            QStringList() << "J" << "I1lumina2",
           "Illumina read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataIllumina3(
            QStringList() << "K" << "I1lumina3",
           "Illumina read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio1(
            QStringList() << "P" << "dataPacBio1",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio2(
            QStringList() << "Q" << "I1lumina3",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio3(
            QStringList() << "R" << "I1lumina3",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio4(
            QStringList() << "S" << "I1lumina3",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio5(
            QStringList() << "T" << "I1lumina3",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");
const QCommandLineOption dataPacBio6(
            QStringList() << "U" << "I1lumina3",
           "PacBio read, Locus file from VarientLister",
           "filename",
            "");

const QCommandLineOption lowersLocus(
            QStringList() << "l" << "lowerst-locus",
           "Start looking for pSNPS from this locus",
           "int",
            "0");
const QCommandLineOption upperLocus(
            QStringList() << "u" << "upper-locus",
           "Stop looking for pSNPs after this locus",
           "int",
            "8916");

const QCommandLineOption titleRoc(
            QStringList() << "t" << "title",
           "Title of ROC curve",
           "stgring",
            "");

#endif // MAIN_H
