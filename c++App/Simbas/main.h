#ifndef MAIN_H
#define MAIN_H

#include <QCommandLineParser>
#include <string>

using namespace std;

static const long double Ten = 10.0;
static const long double One = 1.0;
static const long double Zero = 0.0;

//Input
const QCommandLineOption inFastaTemplate(
            QStringList() << "t" << "template-fasta",
            "Template sequence to base reads upon",
            "filename",
            "t.fasta");

//Paramters
const QCommandLineOption depth(
            QStringList() << "d" << "depth",
            "Depth of simulated sequence",
            "int",
            "100");

//Output
const QCommandLineOption outFilePrefix(
            QStringList() << "p" << "out-FilePrefix",
            "Output prefix for bax.h5 and fastq filenames",
            "filename",
            "simbas");

void configCommandLineParser(QCommandLineParser& parser);
void runProgram(QCommandLineParser& parser);

std::string getFileContents(const char *filename);
void basesFromFasta(string &t);
long double phred2prob(int phred);
int prob2phred(long double prob);




#endif // MAIN_H
