#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QString>
#include <fstream>
#include <string>
#include <cerrno>
#include "varientcaller.h"
#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser clp;
    configCommandLineParser(clp);
    clp.process(a);

    try
    {
        runProgram(clp);
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("VarientLister");
    QCoreApplication::setApplicationVersion("1.1.0.2");

    //QCommandLineParser parser;
    parser.setApplicationDescription("Generate pSNPs from BAM format file");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(bamFilename);
    parser.addOption(templateFilename);
    parser.addOption(readCsvOutfile);
    parser.addOption(lociCsvOutfile);
    parser.addOption(fisherFilename);
    parser.addOption(bionomialFilename);
    parser.addOption(poissonFilename);
    parser.addOption(poissonBinomialFilename);
    parser.addOption(errorThreshold);
    return;
}

void runProgram(QCommandLineParser& clp)
{
    VarientCaller vc(clp.value(bamFilename).toStdString(),
                     clp.value(templateFilename).toStdString(),
                     clp.value(readCsvOutfile).toStdString(),
                     clp.value(lociCsvOutfile).toStdString(),
                     clp.value(fisherFilename).toStdString(),
                     clp.value(bionomialFilename).toStdString(),
                     clp.value(poissonFilename).toStdString(),
                     clp.value(poissonBinomialFilename).toStdString(),
                     clp.value(errorThreshold).toInt());
    vc.write();
}


// http://insanecoding.blogspot.co.uk/2011/11/how-to-read-in-file-in-c.html
std::string getFileContents(const char *filename)
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
    for (unsigned int i = 2 ; i <= N ; i++)
        log_fac += log10((long double)i);
    return log_fac;
}

long double log_sfac(unsigned int N, unsigned  int NpK)
{
    long double log_fac = 0.0;
    for (unsigned int i = N+1 ; i <= NpK ; i++)
        log_fac += log10((long double)(i));
    return log_fac;
}

// http://stackoverflow.com/questions/15301885/calculate-value-of-n-choose-k
//int n_C_r(int n, int r)
//{
//    if (r == 0) return 1;
//   return (n * n_C_r(n - 1, r - 1)) / r;
//}

int factorial(unsigned int N)
{
    int fac = 1;
    for (unsigned int i = 1 ; i <= N ; i++)
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


















