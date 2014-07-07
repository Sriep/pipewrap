#ifndef MAIN_H
#define MAIN_H
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <string>
using namespace std;

void configCommandLineParser(QCommandLineParser& parser);
static string programName = "plotroc";
static string version = "1.0.0.0";

const QCommandLineOption inPvPath(
            QStringList() << "i" << "in-sufix",
            "File contianing list of pvalues",
            "filename",
            "");
const QCommandLineOption inpSNPs(
            QStringList() << "s" << "pSNPs",
           "Output file from pSNPgenerator.R",
           "filename",
            "changes.csv");



#endif // MAIN_H
