#ifndef MAIN_H
#define MAIN_H
#include <QCoreApplication>
//#include <QCommandLineParser>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
static const long double Ten = 10.0;
static const long double One = 1.0;
static const long double Zero = 0.0;

static const string programName = "VarientLister";
static const string version = "1.1.0.0";

//-i big_simbas_blasr.bam -t Sc8915bp_cs.fasta -l locinfo.csv -k h5_x1000_ -5  big_simbas.bax.h5

//void configCommandLineParser(QCommandLineParser& parser);
void runProgram();//QCommandLineParser& parser);
bool compareBases(char c1, char c2);
bool basesDiffer(char c1, char c2);
//long double phred2prob(int phred);

long double kahanSum(const vector<long double>& sumees);

//http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
string getFileContents(const char *filename);

int n_C_r(int n,int r);
//#ifdef LOG_FACTORIALS
static vector<long double> logFactorials;
//#define LOG_FACTORIALS
//#endif
void preCalculateLogFactorials(int numToPrecaculate);
int factorial(int N);
long double log_n_C_r(unsigned int n, unsigned int r);
long double log_fac(unsigned int N);
long double log_sfac(unsigned int N, unsigned int NpK);
long double phred2prob(unsigned char phred);
unsigned int prob2phred(long double prob);

//command line options
//input Bam file
/*const QCommandLineOption bamFilename(
            QStringList() << "i" << "in-Bam",
            "Input BAM file to be searched for pSNPs",
            "filename",
            "stampy_sorted.bam");

//input template fasta file
const QCommandLineOption templateFilename(
            QStringList() << "t" << "template",
           "Input template fasta file",
           "filename",
            "Sc8915bp_cs.fasta");
const QCommandLineOption basH5Filename(
            QStringList() << "5" << "bash5",
           "Input bas.h5 input file",
           "filename",
            "");
const QCommandLineOption freqPartitionFilename(
            QStringList() << "q" << "frequency",
           "File with freqyency of pSNP bins",
           "filename",
            "");
//Structuralparameters
//const QCommandLineOption errorThreshold(
//            QStringList() << "e" << "error-threshold",
//            "Ingnore a read if more than percentage of reads are errors",
//            "percentage",
//            "30");
const QCommandLineOption numFreqPartitionBins(
            QStringList() << "m" << "num-freq-partitions",
            "Number of frequency bins used in FK pvalue estimation",
            "int",
            "100");
const QCommandLineOption precaculateFactorials(
            QStringList() << "c" << "precaculate-factorials",
            "Precalculate factorials to store in look up table."
            "Increase speed of some of the poisson, bionomial and"
            "Fisher exact methods",
            "int",
            "");
// Output options
const QCommandLineOption readCsvOutfile(
            QStringList() << "r" << "readout",
            "Output filename for Read centric output data",
            "filename",
            "read_data.csv");
const QCommandLineOption lociCsvOutfile(
            QStringList() << "l" << "lociout",
            "Output filename for loci centric output data",
            "filename",
            "loci_data.csv");

//output p-values
const QCommandLineOption fisherFilename(
            QStringList() << "f" << "fisher",
            "Output filename prefix for fisher exact test p_values."
            "<.pvalues.csv> stem will be added to filename",
            "filename",
            "");
const QCommandLineOption bionomialFilename(
            QStringList() << "b" << "bionomial",
            "Output filename prefix for bionomial p_values"
            "<.pvalues.csv> stem will be added to filename",
            "filename",
            "");
const QCommandLineOption poissonFilename(
            QStringList() << "p" << "poisson",
            "Output filename prefix for poission p_values"
            "<.pvalues.csv> stem will be added to filename",
            "filename",
            "");
const QCommandLineOption poissonBinomialFilename(
            QStringList() << "n" << "poisson-bionomial",
            "Output filename prefix for poission bionomial p_values"
            "<.pvalues.csv> stem will be added to filename",
            "filename",
            "");
const QCommandLineOption knownFrequencyFilename(
            QStringList() << "k" << "known-frequency",
            "Output filename prefix for known freqyency p_values"
            "<.pvalues.csv> stem will be added to filename",
            "filename",
            "");*/



























#endif // MAIN_H
