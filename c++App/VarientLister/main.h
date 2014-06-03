#ifndef MAIN_H
#define MAIN_H
#include <QCoreApplication>
#include <QCommandLineParser>
#include <fstream>
#include <string>

using namespace std;

void configCommandLineParser(QCommandLineParser& parser);
void runProgram(const string& in_file,
                const string& tp, int err_t,
                ostream& read_out,
                ostream& loci_out);

//http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
string get_file_contents(const char *filename);

int n_C_r(int n,int r);
int factorial(int N);
long double log_n_C_r(unsigned int n, unsigned int r);
long double log_fac(unsigned int N);
long double log_sfac(unsigned int N, unsigned int NpK);

//input Bam file
const QCommandLineOption in_bam_op(
            QStringList() << "i" << "in-Bam",
            "Input BAM file to be searched for pSNPs",
            "filename",
            "stampy_sorted.bam");

//input template fasta file
const QCommandLineOption template_op(
            QStringList() << "t" << "template",
           "Input template fasta file",
           "filename",
            "Sc8915bp_cs.fasta");

//per read output filename
const QCommandLineOption read_csv_op(
            QStringList() << "r" << "readout",
            "Output filename for Read centric output data",
            "filename",
            "read_data.csv");

//per loci output filename
const QCommandLineOption loci_csv_op(
            QStringList() << "l" << "lociout",
            "Output filename for per locus in template data",
            "filename",
            "loci_data.csv");

#endif // MAIN_H
