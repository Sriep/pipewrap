#ifndef MAIN_H
#define MAIN_H
#include <QCommandLineParser>
#include <QCommandLineOption>


void configCommandLineParser(QCommandLineParser& parser);

const QCommandLineOption inSuffix(
            QStringList() << "i" << "in-sufix",
            "File contianing list of pvalues",
            "filename",
            ".pvalues.csv");
const QCommandLineOption inpSNPs(
            QStringList() << "s" << "pSNPs",
           "Input template fasta file",
           "filename",
            "changes.csv");

#endif // MAIN_H
