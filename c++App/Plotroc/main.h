#ifndef MAIN_H
#define MAIN_H
#include <QCommandLineParser>
#include <QCommandLineOption>


void configCommandLineParser(QCommandLineParser& parser);

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
