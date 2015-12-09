//#include <QCommandLineParser>
//#include <QCommandLineOption>
//#include <QCoreApplication>
//#include <QString>
#include <algorithm>
#include <fstream>
#include <string>
#include <cerrno>
#include "options.h"
#include "varientcaller.h"
#include "main.h"
#include "options.h"

int main(int argc, char *argv[])
{
    try
    {
        try
        {
            Options::readOptions(argc, argv);
            if (Options::get(Options::InBamFile).size() > 0
                && Options::get(Options::TemplateFile).size() > 0)
            {
                if (Options::get(Options::PreCalculateFactorials).size()>0)
                {
                    unsigned int preCalFactorials = stoul(
                                Options::get(Options::PreCalculateFactorials));
                    preCalculateLogFactorials(preCalFactorials);
                }
                runProgram();
            }
        }
        catch (const exception& ex)
        {
            perror(ex.what());
            return errno;
        }
    }
    catch (...)
    {
        perror("");
        return errno;
    }

    return 0;
}

void runProgram()
{
    VarientCaller vc;
    vc();
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

//vector<long double> logFactorials;
void preCalculateLogFactorials(int numToPrecaculate)
{
    for ( int n = 0 ; n <= numToPrecaculate ; n++ )
    {
        logFactorials.push_back(log_fac(n));
    }
}
long double log_fac(unsigned int N)
{
    if (N < logFactorials.size())
    {
        return logFactorials[N];
    }
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

bool compareBases(char c1, char c2)
{
    if (c1 == c2) return true;
    if (('a' == c1 && 'A' == c2) || ('a' == c2 && 'A' == c1)) return true;
    if (('g' == c1 && 'G' == c2) || ('g' == c2 && 'G' == c1)) return true;
    if (('c' == c1 && 'C' == c2) || ('c' == c2 && 'C' == c1)) return true;
    if (('t' == c1 && 'T' == c2) || ('t' == c2 && 'T' == c1)) return true;
    return false;
}

bool basesDiffer(char c1, char c2)
{
    const set<unsigned char> validBases{'a','A','c','C','g','G','t','T'};
    if ((!validBases.count(c1)) || !validBases.count(c2)) return false;
    if (c1 < 'a')
    {
        if (c2 < 'a')
        {
            if (c1 == c2) return false;
        }
        else
        {
            if (c1 == c2 - ('a' - 'A')) return false;
        }
    }
    else
    {
        if (c2 < 'a')
        {
            if (c2 == c1 - ('a' - 'A')) return false;
        }
        else
        {
            if (c1 == c2) return false;
        }
    }
    return true;
}


//http://en.wikipedia.org/wiki/Kahan_summation_algorithm
long double kahanSum(const vector<long double>& sumees)
{
    long double sum = Zero;
    long double c = Zero;            // A running compensation for lost low-order bits.
    for (unsigned int i = 0 ; i < sumees.size() ; i++)
    {
        long double y = sumees[i] - c;     // So far, so good: c is zero.
        long double t = sum + y;    // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;          // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                    // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!
                                    // Next time around, the lost low part will be added to y in a fresh attempt.
    }
    return sum;
}

long double phred2prob(unsigned char phred)
{
    return pow(Ten, -((long double) phred)/Ten);
}
/*
long double phred2prob(int phred)
{
    return pow(Ten, -((long double) phred)/Ten);
}*/
unsigned int prob2phred(long double prob)
{
    const int rtv = -Ten*log10(prob);
    return rtv > 0 ? rtv : 0;
}









































