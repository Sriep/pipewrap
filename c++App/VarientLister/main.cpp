#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QString>
#include <fstream>
#include <string>
#include <cerrno>
#include "varientcaller.h"
#include "main.h"

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);






    //stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
    //string out_file = "varients.csv";
    string read_filename = clp.value("r").toStdString();
    string loci_filename = clp.value("l").toStdString();
    ofstream rout;
    ofstream lout;
    try
    {
        rout.open(read_filename.c_str(), ios_base::out | ios_base::trunc);
        lout.open(loci_filename.c_str(), ios_base::out | ios_base::trunc);
        runProgram(clp.value("i").toStdString(),
                   clp.value("t").toStdString(),
                   clp.value("e").toInt(),
                   rout,
                   lout);
        rout.close();
        lout.close();
    }
    catch (...)
    {
        rout.close();
        lout.close();
        return 1;
    }
    return 0;// a.exec();
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.1.0.0");

    //QCommandLineParser parser;
    parser.setApplicationDescription("Generate pSNPs from BAM format file");
    parser.addHelpOption();
    parser.addVersionOption();

    //-i stampy_sorted.bam -t Sc8915bp_cs.fasta -o varients.csv
    //input Bam file
    const QCommandLineOption in_bam_op(
                QStringList() << "i" << "in-Bam",
                "Input BAM file to be searched for pSNPs",
                "filename",
                "stampy_sorted.bam");
    parser.addOption(in_bam_op);
    //input template fasta file
    const QCommandLineOption template_op(
                QStringList() << "t" << "template",
               "Input template fasta file",
               "filename",
                "Sc8915bp_cs.fasta");
    parser.addOption(template_op);
    //per read output filename
    const QCommandLineOption read_csv_op(
                QStringList() << "r" << "readout",
                "Output filename for Read centric output data",
                "filename",
                "read_data.csv");
    parser.addOption(read_csv_op);
    //per loci output filename
    const QCommandLineOption loci_csv_op(
                QStringList() << "l" << "lociout",
                "Output filename for per locus in template data",
                "filename",
                "loci_data.csv");
    parser.addOption(loci_csv_op);


    //error threshold
    const QCommandLineOption err_th_op(
                QStringList() << "e" << "err-threshold",
                "Ignore reads with errors abvove threhold",
                "percentage",
                "10");
    parser.addOption(err_th_op);

    return;
}

void runProgram(const string& in_file,
                const string& tp,
                int err_t,
                ostream& read_out,
                ostream& loci_out)
{

    VarientCaller vc(in_file, tp, err_t, read_out, loci_out);
    vc.writeReadInfo();
    vc.writeLociInfo();
}


// http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

long double log_n_C_r(unsigned int n,unsigned  int r)
{
    if (r>n) return 0;
    return  log_fac(n)- log_fac(r) - log_fac(n-r);
}

long double log_fac(unsigned int N)
{
    long double log_fac = 0.0;
    for (int i = 2 ; i <= N ; i++)
        log_fac += log10((long double)i);
    return log_fac;
}

long double log_sfac(unsigned int N, unsigned  int NpK)
{
    long double log_fac = 0.0;
    for (int i = N+1 ; i <= NpK ; i++)
        log_fac += log10((long double)(i));
    return log_fac;
}

// http://stackoverflow.com/questions/15301885/calculate-value-of-n-choose-k
//int n_C_r(int n, int r)
//{
//    if (r == 0) return 1;
//   return (n * n_C_r(n - 1, r - 1)) / r;
//}

int factorial(int N)
{
    int fac = 1;
    for (int i = 1 ; i <= N ; i++)
        fac = fac * i;
    return fac;
}

#include <stdio.h>
#include <stdlib.h>
//https://ca.answers.yahoo.com/question/index;_ylt=A9mSs2hbdYtTkgUA3cBLBQx.;_ylu=X3oDMTE1YW9oNnNlBHNlYwNzcgRwb3MDMjIEY29sbwNpcjIEdnRpZANTTUVVSzI3XzE-?qid=20131029185331AAlgCET
int n_C_r( int n, int r )
{
    int i, j, sum, *a;
        if ( r == 0 || n <= r ) return 1;
        a= (int *) calloc( (n+1)*++r, sizeof(int) );
        if ( a == NULL ) return 1;
        for ( i= 0; i <= n; ++i ) a[i*r]= 1;
        if ( n+r >= 3 ) a[r+1]= 1;
        for ( sum= 3, i= 2, j= 1; sum < n+r; )
        {
            int b= i*r+j;
            a[b]= a[b-r-1] + a[b-r];
            if ( (--i < ++j) || (j >= r) )
            {
                if ( ++sum <= n+1 )
                {
                    i= sum - 1;
                    j= 1;
                }
                else
                {
                    i= n;
                    j= sum - n;
                }
            }
        }
        sum= a[r*(n+1)-1];
        free( a );
    return sum;
}


















