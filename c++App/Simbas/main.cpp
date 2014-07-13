#include <QCoreApplication>
#include <QCommandLineParser>
#include "bipolercaller.h"
#include "discretecaller.h"
#include <cmath>
#include <fstream>
#include <cerrno>
#include <array>
#include <exception>
#include "main.h"
#include "simbax.h"
#include "chrono"
#include "ctime"
#include "iostream"
#include <qdebug.h>
#include <cstdlib>
#include "options.h"


int main(int argc, char *argv[])
{
    try
    {
        try
        {
            Options::readOptions(argc, argv);
            if (Options::get(Options::InTemplate).size() > 0)
            {
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
/*
void configCommandLineParser(QCommandLineParser& parser)
{
    QCoreApplication::setApplicationName("Simbas");
    QCoreApplication::setApplicationVersion("1.0.0.0");

    parser.setApplicationDescription("Simulate bax files");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inFastaTemplate);
    parser.addOption(depth);
    parser.addOption(outFilePrefix);

    return;
}*/

void runProgram()
{
    system("pwd");
    //string t = getFileContents(clp.value(inFastaTemplate).toStdString().c_str());
    //string t = getFileContents("/media/sdb2/Projects/Source/Dissertation/c++App/build-Simbas-Desktop_Qt_5_3_0_GCC_64Bit_GDB7_7_1-Debug");
    string t = getFileContents(Options::get(Options::InTemplate).c_str());
    basesFromFasta(t);

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    //SimBax simBax(t
    //              ,clp.value(outFilePrefix).toStdString()
    //              ,clp.value(depth).toInt()
    //              ,getBaseCaller(t, clp));


    SimBax simBax(t, getBaseCaller(t));
    simBax();
    end = chrono::system_clock::now();

    chrono::duration<double> elapsed = end-start;
    qDebug() << "time taken seconds : "  << elapsed.count();
    std::cout << "time taken seconds : " << elapsed.count();
}

unique_ptr<BaseCaller> getBaseCaller(const string& t)
{
    unique_ptr<BaseCaller> baseCaller;
    if (false)
    {
        baseCaller.reset(new BiPolerCaller(t
                                           ,5   //lowDelPhred
                                           ,16  //highDelPhred
                                           ,3   //lowInsPhred
                                           ,17  //highInsPhred
                                           ,6   //lowSubPhred
                                           ,27  //highSubPhred
                                           ,30  //lowDelPct
                                           ,40  //lowInsPct
                                           ,40));   //lowSubPct
    }
    else if (Options::get(Options::PhredDistributions) != "")
    {
        baseCaller.reset(new DiscreteCaller());
    }
    else
    {
        /*
        array<unsigned char, nPhreds> delitionsDist =
        {
            0,0,0,1,2,
            4,3,2,2,1,
            1,1,0,0,0,
            0,70
        };
        array<unsigned char, nPhreds> insertionDist =
        {
            0,0,10,10,7,
            5,4,3,3,3,
            3,3,3,3,4,
            4,5,6,6,7,
            6
        };
        array<unsigned char, nPhreds> mergeDist =
        {
            0,0,0,1,1,  1,1,1,1,1,
            2,2,2,3,3,  3,3,4,3,3,
            3,3,3,3,3,  2,2,2,2,1,
            1,1,1,0,1,  0,0,1,0,0,
            0,0,0,0,1,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            30
        };
        array<unsigned char, nPhreds> substitutionDist =
        {
            0,1,1,1,2,
            4,9,10,8,5,
            3,3,2,2,2,
            2,2,2,2,2,
            2,1,1,1,1,
            1,1,1,1,1,30
        };
        */

        array<unsigned short, nPhreds> delitionsDist =
        {0,
            0,0,0,1,2,
            4,3,2,2,1,
            1,1,0,0,0,
            0,70    //15-16
        };
        array<unsigned short, nPhreds> insertionDist =
        {0,
            //0,0,10,10,7,
            //5,4,3,3,3,
            0,0,0,0,3,
            10,7,3,3,3,
            3,3,3,3,3,
            4,4,4,5,6,
            7,7,6,6     //20-23
        };
        array<unsigned short, nPhreds> mergeDist =
        {
            0,0,0,1,1,  1,1,1,1,1,
            2,2,2,3,3,  3,3,4,3,3,
            3,3,3,3,3,  2,2,2,2,1,
            1,1,1,0,1,  0,0,1,0,0,
            0,0,0,0,1,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            0,0,0,0,0,  0,0,0,0,0,
            30
        };
        array<unsigned short, nPhreds> substitutionDist =
        {0,
            0,0,0,1,1,
            4,9,10,8,5,
            3,3,2,2,2,
            2,2,2,2,2,
            2,1,1,1,1,
            1,1,1,1,1,30 //25-30
        };
        baseCaller.reset(new DiscreteCaller(t
                                           ,delitionsDist
                                           ,insertionDist
                                           ,mergeDist
                                           ,substitutionDist));
    }
    return baseCaller;
}




long double phred2prob(unsigned int phred)
{
    return pow(Ten, -((long double) phred)/Ten);
}

unsigned int prob2phred(long double prob)
{
    const int rtv = -Ten*log10(prob);
    return rtv > 0 ? rtv : 0;
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

  string errstring = string("Error determing file content ")
                    + string(filename) + string(" ");
  //        + perror(errstringHead.c_str());
  runtime_error myex = runtime_error(errstring);
  throw runtime_error(errstring);
}

void basesFromFasta(string& t)
{
    if ('>' == t[0])
        t.erase(0, t.find_first_of('\n'));

    //http://en.cppreference.com/w/cpp/algorithm/remove
    t.erase(std::remove_if(t.begin(),
                              t.end(),
                              [](char x){return std::isspace(x);}),
               t.end());
}











































