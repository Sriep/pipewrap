#include <QCoreApplication>
#include <QCommandLineParser>
#include "bipolercaller.h"
#include <cmath>
#include <fstream>
#include <cerrno>

#include "main.h"
#include "simbax.h"


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
    QCoreApplication::setApplicationName("Simbas");
    QCoreApplication::setApplicationVersion("1.0.0.0");

    parser.setApplicationDescription("Simulate bax files");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(inFastaTemplate);
    parser.addOption(depth);
    parser.addOption(outFilePrefix);

    return;
}

void runProgram(QCommandLineParser& clp)
{
    string t = getFileContents(clp.value(inFastaTemplate).toStdString().c_str());
    basesFromFasta(t);

    SimBax simBax(t
                  ,clp.value(outFilePrefix).toStdString()
                  ,clp.value(depth).toInt()
                  ,getBaseCaller(t, clp));
    simBax();
}

unique_ptr<BaseCaller> getBaseCaller(const string& t, QCommandLineParser& clp)
{
    unique_ptr<BaseCaller> baseCaller;
    baseCaller.reset(new BiPolerCaller(t
                                       ,5   //lowDelPhred
                                       ,16  //highDelPhred
                                       ,3   //lowInsPhred
                                       ,17  //highInsPhred
                                       ,6   //lowSubPhred
                                       ,27  //highSubPhred
                                       ,30  //lowDelPct
                                       ,60  //lowInsPct
                                       ,60));   //lowSubPct
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
  throw(errno);
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










